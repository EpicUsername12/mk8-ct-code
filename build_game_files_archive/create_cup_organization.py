from Crypto.Cipher import AES
import os, sys, struct, random, hashlib
import binascii

cup_pages = []
list_of_cups = []
list_of_tracks_per_cups = []

for i in range(16):
	list_of_cups.append([])
	for j in range(16):
		list_of_cups[i].append([j])


for i in range(16):
	list_of_tracks_per_cups.append([])
	for j in range(16):
		list_of_tracks_per_cups[i].append([])
		for z in range(8):
			list_of_tracks_per_cups[i][j].append([])


def add_cup(page_id, cup_id, cup_name, cup_icon_path):
	list_of_cups[page_id][cup_id] = [cup_name, cup_icon_path]

add_cup(0, 0, "Blue Shell Cup", 	"/vol/content/custom/ym_CupIconTogezo^h.bflim")
add_cup(0, 1, "Bomb Cup", 			"/vol/content/custom/ym_CupIconBomb^h.bflim")
add_cup(0, 4, "Coin Cup", 			"/vol/content/custom/ym_CupIconCoin^h.bflim")
add_cup(0, 5, "Boomerang Cup", 		"/vol/content/custom/ym_CupIconBoomerang^h.bflim")

def add_track(page_id, cup_id, track_id, track_path, track_pic_path, track_name, track_retro=""):
	global list_of_cups, list_of_tracks_per_cups
	list_of_tracks_per_cups[page_id][cup_id][track_id] = [page_id, track_path, track_pic_path, track_retro, track_name]


def get_c_string(arr, offset):
	return arr[offset:len(arr)].split("\x00")[0]

add_track(0, 0, 0, "Gu_SplashCourse", 		"/vol/content/custom/cPict_SplashCourse.bflim", 		"Splash Circuit")
add_track(0, 0, 1, "Gu_RvlCanyon", 			"/vol/content/custom/cPict_RvlCanyon.bflim", 			"Canyon Run")
add_track(0, 0, 2, "Gu_Kentakara", 			"/vol/content/custom/cPict_Kentakara.bflim", 			"Kentakara Raceway")
add_track(0, 0, 3, "Gu_RustyHills", 		"/vol/content/custom/cPict_RustyHills.bflim", 			"Rusty Hills")

add_track(0, 1, 0, "Gu_PdftLikeTheWind", 	"/vol/content/custom/cPict_PDFTLTW.bflim", 				"PDFT LIKE THE WIND")
add_track(0, 1, 1, "Gu_Vancouver", 			"/vol/content/custom/cPict_Vancouver.bflim", 			"Vancouver Velocity")
add_track(0, 1, 2, "Gds_BowserCastle", 		"/vol/content/custom/cPict_Gds_Bowser.bflim", 			"Bowser Castle",		"DS")
add_track(0, 1, 3, "Gu_SkyArena", 			"/vol/content/custom/cPict_SkyArena.bflim", 			"Sky Arena")

add_track(0, 4, 0, "G3ds_DaisyHills", 		"/vol/content/custom/cPict_G3ds_Daisy.bflim", 			"Daisy Hills", 			"3DS")
add_track(0, 4, 1, "Gwii_KongSummit", 		"/vol/content/custom/cPict_Gwii_KongSummit.bflim", 		"DK Summit", 			"Wii")
add_track(0, 4, 2, "Gwii_MapleTreeway", 	"/vol/content/custom/cPict_Gwii_Maple.bflim", 			"Maple Treeway",		"Wii")
add_track(0, 4, 3, "G3ds_Wuhu", 			"/vol/content/custom/cPict_G3ds_Wuhu.bflim", 			"Wuhu Loop",			"3DS")

add_track(0, 5, 0, "G64_BowserCastle", 		"/vol/content/custom/cPict_G64_Bowser.bflim", 			"Bowser Castle", 		"N64")
add_track(0, 5, 1, "Gwii_MarioCircuit", 	"/vol/content/custom/cPict_Gwii_MarioCircuit.bflim", 	"Mario Circuit", 		"Wii")
add_track(0, 5, 2, "Gds_DesertHills", 		"/vol/content/custom/cPict_Gds_Desert.bflim", 			"Desert Hills",			"DS")
add_track(0, 5, 3, "G64_MarioRaceway", 		"/vol/content/custom/cPict_G64_MarioRaceway.bflim", 	"Mario Raceway",		"N64")

string_table = ""
cup_entries = [] # [page_id, cup_id, cup_name_, cup_icon_path_]
track_entries = [] # [page_id, cup_id, track_path, track_pic, track_retro, track_name]

for page in range(16):
	for cup in range(16):
		if len(list_of_cups[page][cup]) == 2:

			# [cup_name, cup_icon_path]
			str_cup_name = len(string_table)
			string_table += list_of_cups[page][cup][0] + "\x00"

			str_cup_icon = len(string_table)
			string_table += list_of_cups[page][cup][1] + "\x00"

			cup_entries.append([page, cup, str_cup_name, str_cup_icon])

			cnt = 0
			for track in range(4):
				# [page_id, track_path, track_pic_path, track_retro, track_name]

				str_track_path_offset = len(string_table)
				string_table += list_of_tracks_per_cups[page][cup][track][1] + "\x00"

				str_track_pic_offset = len(string_table)
				string_table += list_of_tracks_per_cups[page][cup][track][2] + "\x00"

				str_track_retro = len(string_table)
				string_table += list_of_tracks_per_cups[page][cup][track][3] + "\x00"

				str_track_name = len(string_table)
				string_table += list_of_tracks_per_cups[page][cup][track][4] + "\x00"

				track_entries.append([page, cup, str_track_path_offset, str_track_pic_offset, str_track_retro, str_track_name, cnt])

				cnt += 1


# cup_entries = []  [page_id, cup_id, cup_name_, cup_icon_path_]
# track_entries = []  [page_id, cup_id, track_path, track_pic, track_retro, track_name]

num_cups_hardcode = 1

file_data = b"CUP0"
file_data += struct.pack(">I", num_cups_hardcode)
file_data += struct.pack(">I", len(cup_entries))
file_data += struct.pack(">I", len(track_entries))

for cup_entry in cup_entries:
	file_data += struct.pack(">I", cup_entry[0])
	file_data += struct.pack(">I", cup_entry[1])
	file_data += struct.pack(">I", cup_entry[2])
	file_data += struct.pack(">I", cup_entry[3])

for track_entry in track_entries:
	file_data += struct.pack(">I", track_entry[0])
	file_data += struct.pack(">I", track_entry[1])
	file_data += struct.pack(">I", track_entry[2])
	file_data += struct.pack(">I", track_entry[3])
	file_data += struct.pack(">I", track_entry[4])
	file_data += struct.pack(">I", track_entry[5])
	file_data += struct.pack(">I", track_entry[6])

file_data += bytes(string_table, "ascii")

f = open("content/data.cup0", "wb")
f.write(file_data)
f.close()

