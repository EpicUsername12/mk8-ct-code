#ifndef _CUP0_LDR_H
#define _CUP0_LDR_H

typedef class CUP0 CUP0;

#define DEFAULT_CUP0_MAGIC 0x43555030
#define TRACK_MAX_ID 81

#include "patch/filesystem.hpp"
#include "patch/message.hpp"
#include "utils.hpp"

#include <mk8/mk8.h>
#include <mk8/nw/lyt/Texture.hpp>

typedef struct CUP0_Header {
    uint32_t magic;
    size_t numPages;
    size_t numCupEntries;
    size_t numTrackEntries;
} CUP0_Header;

typedef struct CUP0_CupEntries {
    int pageId;
    int cupId;
    uint32_t cupNameStr;
    uint32_t cupIconStr;
} CUP0_CupEntries;

typedef struct CUP0_TrackEntries {
    int pageId;
    int cupId;
    uint32_t trackPathStr;
    uint32_t trackPicStr;
    uint32_t trackRetroStr;
    uint32_t trackNameStr;
    uint32_t trackId;
} CUP0_TrackEntries;

typedef struct CUP0_Pages {
    bool pageCustomCups[8];
    nw::lyt::TextureInfo* cupBflimFile[8];
    nw::lyt::TextureInfo* trackBflimFiles[8][4];
    int cupName[8];
    int trackNames[8][4];
    int retroTrackNames[8][4];
    char* trackPaths[8][4];
} CUP0_Pages;

namespace ctgp {

class CUP0 : ctgp::common::RootClass {
  public:
    uint8_t* data = NULL;
    size_t numPages = 0;
    CUP0_Pages** pages = NULL;

    char** trackNames = (char**)(0x101ab678 + MK8_DATA_OFFSET);
    int* cupMessageId = (int*)(0x100D3E84 + MK8_DATA_OFFSET);
    char* trackNamesMod[TRACK_MAX_ID] = { 0 };
    int cupMessageIdSave[12];

    nw::lyt::TextureInfo* cupBflimFile[8];
    nw::lyt::TextureInfo cupBflimFileSave[8];

    CUP0();

    /**
     * @brief Initialize cup0 data
     *
     */
    virtual void Initialize();

    /**
     * @brief Create a Instance object of CUP0
     *
     * @return ctgp::CUP0*
     */
    static inline ctgp::CUP0* createInstance() {
        ctgp::CUP0* inst = new ctgp::CUP0();
        inst->Initialize();
        return inst;
    }
};

///! CUP0 instance
extern CUP0* CUP0Instance;
} // namespace ctgp

#endif