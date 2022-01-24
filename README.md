
# MK8 WiiU CTGP-Cafe Code
  
Initial release

# What's required

For C/C++:

- devkitPro, devkitPPC, devkitARM, libogc, [**libtomcrypt**](https://mega.nz/file/oRdXjAjY#DyNTulpwgRcAmW04RqA72wXtdXSsJnOCKdVKTeYqJD8),  [**xxd.exe**](https://mega.nz/file/wMNDEQSZ#ZzZgvG3BlDA1zDsDtYYV5hj7o2p3kC5HAg5Q9Ez3mRM)

For Python3:

- pycryptodome, paramiko, lxml, arc4, requests

# Preparation

``cd build_game_files_archive``
``mkdir content``
``mkdir file_data``

- Download [**this archive**](https://mega.nz/file/5RFSQDZK#rj7IlSGKvwKa6GNfgZte0a04BBtAKacG45YwfBSs6SQ) and put the files in the ``content`` folder.

# Building

- Simply run ``make`` in the directory, you should get a .elf, you can start it with HBL (even from Wiiload)

# If you want to edit CTGP game files

- Edit game files in ``/content``

- Go in ``/build_game_files_archive``

- Run ``python create_cup_organization.py``

- Run ``python create_archive.py``

- Put content from ``file_data`` on your SDCard at ``root:/ctgp8/file_data`` and you're good to go!