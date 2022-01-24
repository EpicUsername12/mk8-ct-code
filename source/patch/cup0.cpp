#include "cup0.hpp"

ctgp::CUP0 *ctgp::CUP0Instance = NULL;

ctgp::CUP0::CUP0() {
  this->setName("ctgp::CUP0");
  memcpy(this->trackNamesMod, this->trackNames, sizeof(this->trackNamesMod));
  memcpy(this->cupMessageIdSave, this->cupMessageId,
         sizeof(this->cupMessageIdSave));
}

void ctgp::CUP0::Initialize() {

  this->data =
      (uint8_t *)GetArchiveFile("/vol/content/data.cup0", NULL, 0x1000);
  CUP0_Header *header = (CUP0_Header *)this->data;
  if (header->magic != DEFAULT_CUP0_MAGIC) {
    FatalError("444-0000", "One of the file is heavily corrupted, please check "
                           "your SDCard or reinstall CTGP-Café");
    return;
  }

  this->numPages = header->numPages;
  this->pages = (CUP0_Pages **)calloc(this->numPages, sizeof(CUP0_Pages *));
  for (size_t i = 0; i < this->numPages; i++)
    this->pages[i] = (CUP0_Pages *)calloc(1, sizeof(CUP0_Pages));

  char *stringTable =
      (char *)(this->data + sizeof(CUP0_Header) +
               (sizeof(CUP0_CupEntries) * header->numCupEntries) +
               (sizeof(CUP0_TrackEntries) * header->numTrackEntries));
  CUP0_CupEntries *cupEntries =
      (CUP0_CupEntries *)(this->data + sizeof(CUP0_Header));
  CUP0_TrackEntries *trackEntries =
      (CUP0_TrackEntries *)(this->data + sizeof(CUP0_Header) +
                            (sizeof(CUP0_CupEntries) * header->numCupEntries));

  for (size_t i = 0; i < header->numCupEntries; i++) {
    int pageId = cupEntries[i].pageId;
    int cupId = cupEntries[i].cupId;
    char *cupName = &stringTable[cupEntries[i].cupNameStr];
    char *cupIcon = &stringTable[cupEntries[i].cupIconStr];
    AddMessage(fromUTF8(cupName), 14337 + cupId, true);
    this->pages[pageId]->cupName[cupId] = 14337 + cupId;
    this->pages[pageId]->pageCustomCups[cupId] = true;

    uint32_t outSize = 0;
    uint8_t *cupData = (uint8_t *)GetArchiveFile(cupIcon, &outSize, 0x1000);
    this->pages[pageId]->cupBflimFile[cupId] =
        (nw::lyt::TextureInfo *)calloc(1, sizeof(nw::lyt::TextureInfo));
    if (!nw::lyt::LoadTexture(this->pages[pageId]->cupBflimFile[cupId], cupData,
                              outSize)) {
      FatalError("444-0001", "One of the file is heavily corrupted, please "
                             "check your SDCard or reinstall CTGP-Café");
      return;
    }
  }

  for (size_t i = 0; i < header->numTrackEntries; i++) {

    int pageId = trackEntries[i].pageId;
    int cupId = trackEntries[i].cupId;
    char *trackName = &stringTable[trackEntries[i].trackNameStr];
    char *trackNameRetro = &stringTable[trackEntries[i].trackRetroStr];
    char *trackPath = &stringTable[trackEntries[i].trackPathStr];
    char *trackPic = &stringTable[trackEntries[i].trackPicStr];

    this->pages[pageId]->retroTrackNames[cupId][trackEntries[i].trackId] = -1;
    this->pages[pageId]->trackPaths[cupId][trackEntries[i].trackId] = trackPath;

    AddMessage(fromUTF8(trackName), 15337 + i, true);
    this->pages[pageId]->trackNames[cupId][trackEntries[i].trackId] = 15337 + i;

    if (strlen(trackNameRetro) > 0) {
      AddMessage(fromUTF8(trackNameRetro), 15937 + i, true);
      this->pages[pageId]->retroTrackNames[cupId][trackEntries[i].trackId] =
          15937 + i;
    }

    uint32_t outSize = 0;
    uint8_t *trackData = (uint8_t *)GetArchiveFile(trackPic, &outSize, 0x1000);
    this->pages[pageId]->trackBflimFiles[cupId][trackEntries[i].trackId] =
        (nw::lyt::TextureInfo *)calloc(1, sizeof(nw::lyt::TextureInfo));
    if (!nw::lyt::LoadTexture(
            this->pages[pageId]
                ->trackBflimFiles[cupId][trackEntries[i].trackId],
            trackData, outSize)) {
      FatalError("444-0002", "One of the file is heavily corrupted, please "
                             "check your SDCard or reinstall CTGP-Café");
      return;
    }
  }

  CUP0Instance = this;
}