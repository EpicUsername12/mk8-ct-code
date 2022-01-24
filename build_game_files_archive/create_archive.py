# Running this under: Python 3.9.0 (tags/v3.9.0:9cf6752, Oct  5 2020, 15:34:40) [MSC v.1927 64 bit (AMD64)] on win32
#
#	The goal is to create an encrypted archive file, that will hold our game files we want to add/replace
#	Header -> Magic, Num Entries, Version, Unencrypted Data SHA1
#	Entry  -> Magic, Name hash, file size, file offset, per file 128bit seed
#
import paramiko
import os, hashlib, shutil
from lxml import etree
from arc4 import ARC4
from zipfile import ZipFile

import requests
import urllib3
urllib3.disable_warnings()

def get_file(filename):
	f_ = open(filename, "rb+")
	data = f_.read()
	return f_, data, len(data)

def calc_hash(filename, multiplier):
	res = 0
	for c in filename:
		res = ord(c) + res * multiplier
	return res & 0xFFFFFFFF

# version_num = 0xAB MA VV MI
# AB = alpha beta 
# MA =  major version
# VV = version
# MI = minor version

alpha = 0
beta = 1
major = 1
version = 1
minor = 0

version_num = (alpha << 28) | (beta << 24) | (major << 16) | (version << 8) | minor

list_of_files = []

private_ctgp_key = [
	0xB3, 0x74, 0xC0, 0x03, 0x35, 0x84, 0x94, 0xDC, 0x1B, 0xCF, 0x7E, 0x8C,
	0xE2, 0x96, 0x87, 0x6F, 0x08, 0x06, 0xD9, 0xDB, 0xB6, 0x9E, 0xE1, 0xFE,
	0x46, 0x74, 0x13, 0x7D, 0x6E, 0x95, 0xDF, 0xED, 0x46, 0x2B, 0xC4, 0x2C,
	0xBA, 0x16, 0xCB, 0xA1, 0x17, 0xB4, 0xEC, 0x96, 0xB5, 0x5B, 0x31, 0x9C]

tree = etree.Element("list")

if os.path.isdir("file_data"):
	shutil.rmtree("file_data")
os.makedirs("file_data", 0o777, True)

zipobj = ZipFile("release.zip", "w")

hash_list = []
list_version_local = []
list_version_server = []

idx = 0
for path, subdirs, files in os.walk("content/"):
	for name in files:

		entry = etree.SubElement(tree, "entry")

		fpath = os.path.join(path, name).replace("\\", "/")
		fd, fbuf, flen = get_file(fpath)

		rx = ARC4(bytes(private_ctgp_key))
		fenc = rx.decrypt(fbuf)

		hash = calc_hash(fpath, 0x477)
		hash_list.append(hash)

		m = hashlib.sha1()
		m.update(fenc)

		print("Writing file: " + fpath + " ({:08X}".format(hash) + ".dat)")

		str_hash = ""
		sha1_hash = bytearray(m.digest())
		for i in range(len(sha1_hash)):
			sha1_hash[i] ^= 0x99
			str_hash += "{:02x}".format(sha1_hash[i])

		entry.set("id", str(idx))
		entry.set("file", "{:08X}".format(hash))
		entry.set("value", str_hash)

		list_version_local.append(["{:08X}".format(hash), str_hash])

		f = open("file_data/{:08X}.dat".format(hash), "wb")
		f.write(fenc)
		f.close()

		zipobj.write("file_data/{:08X}.dat".format(hash))

		idx+=1

ver_list = open("ctgp_filelist.ver", "wb")
ver_list.write(etree.tostring(tree, pretty_print=True))
ver_list.close()
zipobj.close()

print("")

for hash in hash_list:
	print("0x{:08X}, ".format(hash), end='')

print("")
print("")
print("")

xml_ver = requests.get("https://rambo6dev.net/ctgp/ctgp_filelist.ver", verify=False).content
root = etree.fromstring(xml_ver)
for watt in root.iter():
	if watt.tag == "entry":
		list_version_server.append([watt.attrib["file"], watt.attrib["value"]])

diff_list = []

for i in range(len(list_version_local)):

	if i < len(list_version_server):
		ent_local = list_version_local[i]
		ent_server = list_version_server[i]

		if ent_local[1] != ent_server[1]:
			diff_list.append(ent_local[0])
	else:
		diff_list.append(ent_local[0])

if len(diff_list) == 0:
	exit(0)

for e in diff_list:
	print("Updates: file_data/" + e + ".dat")
print("")


choice = input("Do you want to push updated files (y/n): ")
if not "y" in choice:
	exit(0)

passw = input("Ok, then enter server password: ")
if "T+uJu7WbkUu&u;2k" not in passw:
	exit(0)

print("")

host = "51.91.56.16"
port = 22
transport = paramiko.Transport((host, port))

password = "T+uJu7WbkUu&u;2k"
username = "ubuntu"

transport.connect(username = username, password = password)

sftp = paramiko.SFTPClient.from_transport(transport)
sftp.put(os.getcwd() + "/ctgp_filelist.ver", "/var/www/non_host/ctgp/ctgp_filelist.ver")
for e in diff_list:
	print("Uploading: file_data/" + e + ".dat")
	sftp.put(os.getcwd() + "/file_data/" + e + ".dat", "/var/www/non_host/ctgp/file_data/" + e + ".dat")

transport.close()
sftp.close()
