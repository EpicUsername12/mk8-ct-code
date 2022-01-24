#include "filesystem.hpp"

OSMutex fs_mutex;

uint8_t private_ctgp_key[48] = { 0xB3, 0x74, 0xC0, 0x03, 0x35, 0x84, 0x94, 0xDC, 0x1B, 0xCF, 0x7E, 0x8C, 0xE2, 0x96, 0x87, 0x6F, 0x08, 0x06, 0xD9, 0xDB, 0xB6, 0x9E, 0xE1, 0xFE,
                                 0x46, 0x74, 0x13, 0x7D, 0x6E, 0x95, 0xDF, 0xED, 0x46, 0x2B, 0xC4, 0x2C, 0xBA, 0x16, 0xCB, 0xA1, 0x17, 0xB4, 0xEC, 0x96, 0xB5, 0x5B, 0x31, 0x9C };

uint32_t fileHashes[] = {
    0x41B15EF8, 0x30B81E30, 0xD22AD39E, 0x4FB8C892, 0xA0A48549, 0x4C220436, 0x5B12E565, 0x43A28B42, 0xEC672A77, 0xE05BF7C0, 0xD71A6E44, 0x07E304F3, 0x7AA76DE3, 0x622CA0DF,
    0x17529664, 0x3DAAB95B, 0x1F51AF11, 0x4614E8D0, 0x90CF3D56, 0xE081DDD6, 0xD3ECA2E2, 0x5F5C4C17, 0x1AF82720, 0x5F17ABE4, 0x0E6BE493, 0x1405AAF2, 0xD48056C9, 0xAA6EAF3E,
    0xB28AB7B3, 0xBB397433, 0x3AB78964, 0xD6782E80, 0x9F78DBEF, 0xC176AF0A, 0xB888BF30, 0x52670A87, 0x5942190C, 0x2F327243, 0x78967306, 0xED3C8808, 0x8BA0A357, 0xE2C6EE35,
    0x43CC7F34, 0xECC2D06F, 0x2AF90ECE, 0x4D33320D, 0x7588B65A, 0x872C230F, 0xADAF52E8, 0xE5C1BFDC, 0xEE48BE0B, 0xD4D366A6, 0xF4BE0592, 0x07145900, 0x4CF124EC, 0xB1152FC1,
    0x4BDAFF06, 0x643E674E, 0x56FF9506, 0x5243FFB4, 0xE4137B60, 0xA25CDF75, 0x5C8C0572, 0x9C756A82, 0x58F89E21, 0x92D42CBB, 0x6EA9F600, 0x31D3501F, 0xA1FB392D, 0xBBEC2E04,
    0xC82E5892, 0x361F8C29, 0xCDC75ADE, 0x8370FF13, 0xCEE869D3, 0x803504C4, 0xF3CEC020, 0xBC1B3798, 0x87F01B66, 0x603C6CB5, 0xC9024372, 0x37756BDF, 0x7DB79DA7, 0xA9857C10,
    0xA3385374, 0x52C82626, 0x2E3AA2D4, 0xD6FF18D7, 0x343C6480, 0xF7FF0161, 0x51AFAA95, 0xB1D4C752, 0x75C1E1A2, 0x772D4AF3, 0x4670DC71, 0xF11A54D2, 0x85EAAF0D, 0xA92DDCEC,
    0xFE42B272, 0xCC9A227D, 0xD0F6785F, 0xB27A1E1C, 0xEA890A2A, 0x990E79D1, 0x5B62FCF6, 0x26BFC96B, 0x63D790EC, 0x280318B8, 0x3D4172B9, 0xEE69504A, 0x496C9ED5, 0x925372E4,
    0xB22F6175, 0xDEA773A7, 0x79E22646, 0x658400C1, 0xB48A6AC7, 0xF3B63E83, 0xD3309D17, 0x503C3675, 0xC11C1B74, 0x016DD4AF, 0x5F927794, 0x2625FE17, 0x8F353D40, 0x250EC2A1,
    0x80D6EF55, 0x6A1C2092, 0xFDCBEE62, 0x7EE29071, 0x71C562CC, 0xB0FCCA3F, 0xAF97F87B, 0xA86DFE5F, 0x25AC08ED, 0x4DE3BB6C, 0xF466B7A7, 0x44CAC62B, 0x948CE248, 0x5C18B2A7,
    0xF09FF182, 0x125F58AC, 0x47EE0653, 0x2FFCAC59, 0xFAFD4498, 0xA9BBF053, 0xEC5C3077, 0xBECE7B9C, 0xE7BC1D33, 0x6740A896, 0x7F1FF9F8, 0x18CBD6E7, 0xDDF85A25, 0xE504B624,
    0xE1E8ED5F, 0x0F752A7A, 0x7352BF01, 0x65DF2446, 0x95A53E6B, 0xD375EDBB, 0x8169E11C, 0x2ADE9B08, 0x014C2AF7, 0xE7271B12, 0xA1FD20A1, 0x5A4C843D, 0xE9FCB63C, 0x7904C1CD,
    0x9AF9AA8F, 0xE4601DAE, 0xD500F129, 0x70D4EC4F, 0x35DD2D04, 0x1705158B, 0x058AB3D0, 0xCCA2C2CF, 0xD80CE4FD, 0x812C4C7C, 0xD83662B7, 0xF1A9F516, 0x6D075D85, 0xECF1F622,
    0xAAAC3BAF, 0xFA33F357, 0x87D4C1E0, 0x4AB50B24, 0x98E3C7D3, 0xFB17856E, 0xAD37695E, 0xD7FA51B3, 0xAEE3E608, 0xFADDF204, 0x6C5ED9FF, 0x9515DEF6, 0x9D3E729C, 0xBDF7C6E1,
    0xADB13C53, 0xACF1E636, 0x4F01F4E3, 0x5859FDC1, 0xF8913D2D, 0x51F5F81B, 0xF479C545, 0x0E7E0966, 0x2EA04728, 0x133573F2, 0x956F8FA9, 0x383A0EA5, 0xD7B70ABC, 0x54B0F2E6,
    0xB57E039A, 0x44A54FE9, 0xD5606736, 0xD5C22676, 0x86B542CD, 0x3D2DABA3, 0x64AC87F8, 0xB2837E62, 0x5C99A527, 0xEAC0EBDF, 0x74F0D881, 0x1E3C503F, 0x7DFEE1B7, 0x4BE3DB6F,
    0x50AF2C0F, 0x2E457181, 0x6E394597, 0xD3EA7E1F, 0x2B80BE8B, 0x7F2AE59D, 0xD2D50CAF, 0x267F33C1, 0x47C04277, 0x795B2D4F, 0xEDD58589, 0x622FCB1F, 0xB0FE0653, 0x26B1E88A,
    0x016F8E07, 0x512F23DF, 0x6F51CE47, 0x37579788, 0x27A77A35, 0xF7F7BA47, 0x3EB51EFF,
};
/* internal save file var */
uint32_t* iSaveFileHeader = NULL; // for crc32 calc
int iOutSaveFileHandle = -1;
size_t iRealSaveFileSize = 0;

FSClient* fsClient = NULL;
FSCmdBlock* fsCmdBlock = NULL;
char outSdPath[0x300];
char mountPath[128];

struct FileRedirections {

    fileHandleHook hooks[256];

    fileHandleHook* Add(int h) {
        for (int i = 0; i < 256; i++) {
            if (!hooks[i].active) {
                hooks[i].active = true;
                hooks[i].handle = h;
                hooks[i].offset = 0;
                return &hooks[i];
            }
        }

        FatalError("444-0100", "There's too many file for CTGP to handle!");
        return NULL;
    }

    fileHandleHook* Find(int h) {

        for (int i = 0; i < 256; i++) {
            if (hooks[i].active && hooks[i].handle == h) {
                return &hooks[i];
            }
        }

        return NULL;
    }
};

FileRedirections redirection;

uint32_t calc_hash(const char* filename, uint32_t multiplier) {
    uint32_t res = 0;
    for (size_t i = 0; i < strlen(filename); i++) {
        res = filename[i] + res * multiplier;
    }
    return res;
}

uint8_t* GetArchiveFile(const char* filename, uint32_t* outSize, uint32_t align) {

    int fd = -1;
    FSStat s;

    int ret = FSOpenFile(fsClient, fsCmdBlock, filename, "r", &fd, -1);

    if (ret < 0 || fd < 0) {
        FatalError("444-0102", "Error, one of the file is missing, please check your installation");
        return NULL;
    }

    FSGetStatFile(fsClient, fsCmdBlock, fd, &s, -1);
    uint8_t* data = (uint8_t*)memalign(align, (s.size + 0x100) & (~0xFF));
    int size_r = FSReadFile(fsClient, fsCmdBlock, data, 1, s.size, fd, 0, -1);
    FSCloseFile(fsClient, fsCmdBlock, fd, -1);
    DCFlushRange(data, size_r);

    if (outSize)
        *outSize = size_r;
    return data;
}

DECL_HOOK(int, FSOpenFile, FSClient* client, FSCmdBlock* block, const char* path, const char* mode, int* handle, uint32_t flags) {

    char* nPath = (char*)calloc(256, 1);
    if (ctgp::CUP0Instance) {
        for (int i = 0; i < 80; i++) {
            if (ctgp::CUP0Instance->trackNames[i]) {
                char* x1 = strstr(path, ctgp::CUP0Instance->trackNames[i]);
                char* x2 = x1 + strlen(ctgp::CUP0Instance->trackNames[i]);
                if (x1 && ctgp::CUP0Instance->trackNamesMod[i]) {
                    DBG_LOG("%s -> %s\n", path, ctgp::CUP0Instance->trackNamesMod[i]);
                    memcpy(nPath, path, strlen(path));
                    memcpy(nPath + ((int)x1 - (int)path), ctgp::CUP0Instance->trackNamesMod[i], strlen(ctgp::CUP0Instance->trackNamesMod[i]) + 1);
                    memcpy(nPath + strlen(nPath), x2, strlen(x2));
                    nPath[((int)x1 - (int)path) + strlen(ctgp::CUP0Instance->trackNamesMod[i]) + strlen(x2)] = 0;
                    break;
                }
            }
        }
    }

    uint32_t hash = calc_hash((strlen(nPath) > 0) ? (nPath + 5) : (path + 5), 0x477);
    char hash_filename[256];
    __os_snprintf(hash_filename, 256, "%s/ctgp8/file_data/%08X.dat", mountPath, hash);

    bool allowedHash = false;
    for (size_t i = 0; i < (sizeof(fileHashes) / sizeof(uint32_t)); i++) {
        if (fileHashes[i] == hash) {
            allowedHash = true;
            break;
        }
    }

    int ret = -1;
    int hashFd = -1;
    ret = real_FSOpenFile(client, block, hash_filename, "r", &hashFd, -1);
    if (hashFd >= 0 && allowedHash) {
        redirection.Add(hashFd);
        *handle = hashFd;
        free(nPath);
        return ret;
    } else if (hashFd <= 0 && allowedHash) {
        FatalError("444-0103", "One of the CTGP files is missing, please check your installation");
    }

    free(nPath);
    return real_FSOpenFile(client, block, path, mode, handle, flags);
}

DECL_HOOK(int, FSReadFile, FSClient* client, FSCmdBlock* block, void* buffer, uint32_t count, size_t size, int handle, uint32_t unk1, uint32_t flags) {

    fileHandleHook* hook = redirection.Find(handle);
    if (hook) {
        int size_r = real_FSReadFile(client, block, buffer, count, size, handle, unk1, flags);
        ctgp::crypto::CryptoEngineInstance->RC4_EncryptDecrypt(private_ctgp_key, sizeof(private_ctgp_key), (uint8_t*)buffer, (uint8_t*)buffer, size_r, hook->offset);
        hook->offset += size_r;
        return size_r;
    }

    return real_FSReadFile(client, block, buffer, count, size, handle, unk1, flags);
}

DECL_HOOK(int, FSReadFileWithPos, FSClient* client, FSCmdBlock* block, uint8_t* buffer, uint32_t count, size_t size, uint32_t pos, int handle, uint32_t unk1, uint32_t flags) {

    fileHandleHook* hook = redirection.Find(handle);
    if (hook) {
        int size_r = real_FSReadFileWithPos(client, block, buffer, count, size, pos, handle, unk1, flags);
        ctgp::crypto::CryptoEngineInstance->RC4_EncryptDecrypt(private_ctgp_key, sizeof(private_ctgp_key), (uint8_t*)buffer, (uint8_t*)buffer, size_r, pos);
        hook->offset = pos + size_r;
    }

    return real_FSReadFileWithPos(client, block, buffer, count, size, pos, handle, unk1, flags);
}

DECL_HOOK(int, FSSetPosFile, FSClient* client, FSCmdBlock* block, int handle, uint32_t pos, uint32_t flags) {

    fileHandleHook* hook = redirection.Find(handle);
    if (hook) {
        hook->offset = pos;
        return real_FSSetPosFile(client, block, handle, pos, flags);
    }
    return real_FSSetPosFile(client, block, handle, pos, flags);
}

DECL_HOOK(int, FSGetStat, FSClient* client, FSCmdBlock* block, const char* path, FSStat* stat, uint32_t flags) {

    uint32_t hash = calc_hash(path + 5, 0x477);
    char hash_filename[256];
    __os_snprintf(hash_filename, 256, "%s/ctgp8/file_data/%08X.dat", mountPath, hash);

    int ret = -1;
    int hashFd = -1;
    ret = real_FSOpenFile(client, block, hash_filename, "r", &hashFd, -1);

    if (hashFd >= 0) {
        ret = FSGetStatFile(client, block, hashFd, stat, flags);
        FSCloseFile(client, block, hashFd, -1);
        return ret;
    }

    return real_FSGetStat(client, block, path, stat, flags);
}

int __check_sd_access() {

    int ret = FSGetMountSource(fsClient, fsCmdBlock, 0, outSdPath, -1);
    R_UNLESS(ret >= 0, -1);

    ret = FSMount(fsClient, fsCmdBlock, outSdPath, mountPath, 128, -1);
    R_UNLESS(ret >= 0, -2);

    return ret;
}

static int iAreWePatched = 0;
PatchedFunction* listFuncPatch[32];

DECL_HOOK(ProcUIStatus, ProcUIProcessMessages, bool block) {
    ProcUIStatus st = real_ProcUIProcessMessages(block);
    if (st == PROCUI_STATUS_RELEASE_FOREGROUND || st == PROCUI_STATUS_EXITING) {
        for (int i = 0; i < 32; i++) {
            if (listFuncPatch[i]) {
                removePatch(listFuncPatch[i]);
                listFuncPatch[i] = NULL;
            }
        }
        iAreWePatched = 0;
    }

    if ((st == PROCUI_STATUS_IN_FOREGROUND) && (iAreWePatched == 0)) {
        listFuncPatch[0] = HOOK_FUNC(FSOpenFile, FSOpenFile);
        listFuncPatch[1] = HOOK_FUNC(FSReadFile, FSReadFile);
        listFuncPatch[2] = HOOK_FUNC(FSReadFileWithPos, FSReadFileWithPos);
        listFuncPatch[3] = HOOK_FUNC(FSSetPosFile, FSSetPosFile);
        listFuncPatch[4] = HOOK_FUNC(FSGetStat, FSGetStat);
        iAreWePatched = 1;
    }

    return st;
}

void InitializePatchFilesystem() {

    memset(&redirection, 0, sizeof(redirection));
    OSInitMutexEx(&fs_mutex, "Filesystem I/O");

    if (!fsClient)
        fsClient = (FSClient*)malloc(sizeof(FSClient));

    if (!fsCmdBlock)
        fsCmdBlock = (FSCmdBlock*)malloc(sizeof(FSCmdBlock));

    FSInit();
    FSAddClient(fsClient, -1);
    FSInitCmdBlock(fsCmdBlock);
    int sd = __check_sd_access();

    if (sd < 0) {
        char* crashBuffer = (char*)malloc(0x1000);
        if (sd == -1) {
            __os_snprintf(crashBuffer, 0x1000, "Couldn't find a SD Card. (%d)", sd);
            OSFatal(crashBuffer);
        }

        if (sd == -2) {
            __os_snprintf(crashBuffer, 0x1000, "Couldn't mount the SD Card. (%d)", sd);
            OSFatal(crashBuffer);
        }
    } else {
        DBG_LOG("SD Card Mounted to: %s\n", mountPath);
    }

    listFuncPatch[0] = HOOK_FUNC(FSOpenFile, FSOpenFile);
    listFuncPatch[1] = HOOK_FUNC(FSReadFile, FSReadFile);
    listFuncPatch[2] = HOOK_FUNC(FSReadFileWithPos, FSReadFileWithPos);
    listFuncPatch[3] = HOOK_FUNC(FSSetPosFile, FSSetPosFile);
    listFuncPatch[4] = HOOK_FUNC(FSGetStat, FSGetStat);
    iAreWePatched = 1;
    HOOK_FUNC(ProcUIProcessMessages, ProcUIProcessMessages);
}