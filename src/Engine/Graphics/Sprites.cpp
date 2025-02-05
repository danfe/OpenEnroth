#include "Engine/Graphics/Sprites.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "Engine/Engine.h"
#include "Engine/LOD.h"
#include "Engine/OurMath.h"

#include "Engine/Objects/Actor.h"

#include "Engine/Serialization/LegacyImages.h"

#include "Engine/Graphics/DecorationList.h"
#include "Engine/Graphics/PaletteManager.h"


struct SpriteFrameTable *pSpriteFrameTable;

//----- (0044D4D8) --------------------------------------------------------
void SpriteFrameTable::ReleaseSFrames() {
    free(this->pSpriteSFrames);
    this->pSpriteSFrames = nullptr;
    this->uNumSpriteFrames = 0;
}

//----- (0044D4F6) --------------------------------------------------------
void SpriteFrameTable::ResetLoadedFlags() {
    for (int i = 0; i < this->uNumSpriteFrames; ++i) {
        this->pSpriteSFrames[i].uFlags &= ~0x80;
    }
}

//----- (0044D513) --------------------------------------------------------
void SpriteFrameTable::InitializeSprite(signed int uSpriteID) {
    // SpriteFrameTable *v2; // esi@1
    //unsigned int iter_uSpriteID;  // ebx@3
    // char *v4; // edx@3
    // int v5; // eax@3
    //    SpriteFrame *v6; // ecx@5
    //    int v7; // eax@5
    // int16_t v8; // ax@6
    // signed int v9; // edx@6
    // int v10; // ecx@6
    //    signed int v11; // edi@10
    // int16_t v12; // ax@16
    //    int v13; // ecx@16
    // size_t v14; // eax@19
    //    signed int v15; // edi@19
    //    int16_t v16; // ax@27
    //    int v17; // ecx@27
    //    signed int v18; // edi@29
    //    SpriteFrame *v19; // eax@30
    //    int16_t v20; // ax@45
    //    int v21; // ecx@45

    char Str[32];          // [sp+Ch] [bp-3Ch]@19
    char sprite_name[20];  // [sp+2Ch] [bp-1Ch]@15
    char Source[4];        // [sp+40h] [bp-8h]@19

    // v2 = this;
    if (uSpriteID <= this->uNumSpriteFrames) {
        if (uSpriteID >= 0) {
            uint iter_uSpriteID = uSpriteID;
            //if (iter_uSpriteID == 603) __debugbreak();

            int uFlags = pSpriteSFrames[iter_uSpriteID].uFlags;

            if (!(uFlags & 0x0080)) {  // not loaded
                pSpriteSFrames[iter_uSpriteID].uFlags |= 0x80;  // set loaded

                while (1) {
                    pSpriteSFrames[iter_uSpriteID].ResetPaletteIndex(pPaletteManager->LoadPalette(pSpriteSFrames[iter_uSpriteID].uPaletteID));

                    if (uFlags & 0x10) {  // single frame per frame sequence
                        auto v8 = pSprites_LOD->LoadSprite(pSpriteSFrames[iter_uSpriteID].texture_name.c_str(), pSpriteSFrames[iter_uSpriteID].uPaletteID);

                        if (v8 == -1) {
                            logger->Warning("Sprite %s not loaded!", pSpriteSFrames[iter_uSpriteID].texture_name.c_str());
                            for (uint i = 0; i < 8; ++i) {
                                pSpriteSFrames[iter_uSpriteID].hw_sprites[i] = nullptr;
                            }
                        } else {
                            for (uint i = 0; i < 8; ++i) {
                                pSpriteSFrames[iter_uSpriteID].hw_sprites[i] = &pSprites_LOD->pHardwareSprites[v8];
                            }
                        }

                    } else if (uFlags & 0x10000) {
                        for (uint i = 0; i < 8; ++i) {
                            switch (i) {
                                case 3:
                                case 4:
                                case 5:
                                    sprintf(sprite_name, "%s4",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                                case 2:
                                case 6:
                                    sprintf(sprite_name, "%s2",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                                case 0:
                                case 1:
                                case 7:
                                    sprintf(sprite_name, "%s0",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                            }
                            auto v12 = pSprites_LOD->LoadSprite(
                                sprite_name, pSpriteSFrames[iter_uSpriteID].uPaletteID);
                            // pSpriteSFrames[iter_uSpriteID].pHwSpriteIDs[i]=v12;
                            if (v12 == -1) __debugbreak();
                            pSpriteSFrames[iter_uSpriteID].hw_sprites[i] =
                                &pSprites_LOD->pHardwareSprites[v12];
                        }

                    } else if (uFlags & 0x40) {  // part of monster fidgeting seq
                        strcpy(Source, "stA");
                        strcpy(Str, pSpriteSFrames[iter_uSpriteID].texture_name.c_str());
                        auto v14 = strlen(Str);
                        strcpy(&Str[v14 - 3], Source);
                        for (uint i = 0; i < 8; ++i) {
                            switch (i) {
                                case 0:
                                    sprintf(sprite_name, "%s0",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                                case 4:
                                    sprintf(sprite_name, "%s4", Str);
                                    break;
                                case 3:
                                case 5:
                                    sprintf(sprite_name, "%s3", Str);
                                    break;
                                case 2:
                                case 6:
                                    sprintf(sprite_name, "%s2",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                                case 1:
                                case 7:
                                    sprintf(sprite_name, "%s1",
                                            pSpriteSFrames[iter_uSpriteID]
                                                .texture_name.c_str());
                                    break;
                            }
                            auto v12 = pSprites_LOD->LoadSprite(
                                sprite_name, pSpriteSFrames[iter_uSpriteID].uPaletteID);
                            // pSpriteSFrames[iter_uSpriteID].pHwSpriteIDs[i]=v12;
                            if (v12 == -1) __debugbreak();

                            pSpriteSFrames[iter_uSpriteID].hw_sprites[i] =
                                &pSprites_LOD->pHardwareSprites[v12];
                        }
                    } else {
                        for (uint i = 0; i < 8; ++i) {
                            if (((0x0100 << i) &
                                 pSpriteSFrames[iter_uSpriteID].uFlags)) {  // mirrors
                                switch (i) {
                                    case 1:
                                        sprintf(sprite_name, "%s7",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 2:
                                        sprintf(sprite_name, "%s6",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 3:
                                        sprintf(sprite_name, "%s5",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 4:
                                        sprintf(sprite_name, "%s4",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 5:
                                        sprintf(sprite_name, "%s3",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 6:
                                        sprintf(sprite_name, "%s2",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                    case 7:
                                        sprintf(sprite_name, "%s1",
                                                pSpriteSFrames[iter_uSpriteID]
                                                    .texture_name.c_str());
                                        break;
                                }
                            } else {
                                // some names already passed through with codes attached
                                if (strlen(pSpriteSFrames[iter_uSpriteID].texture_name.c_str()) < 7) {
                                    sprintf(sprite_name, "%s%i", pSpriteSFrames[iter_uSpriteID].texture_name.c_str(), i);
                                } else {
                                    sprintf(sprite_name, "%s", pSpriteSFrames[iter_uSpriteID].texture_name.c_str());
                                    // __debugbreak();
                                }
                            }

                            auto v12 = pSprites_LOD->LoadSprite(sprite_name, pSpriteSFrames[iter_uSpriteID].uPaletteID);
                            // pSpriteSFrames[iter_uSpriteID].pHwSpriteIDs[i]=v12;

                            if (v12 == -1) __debugbreak();

                            pSpriteSFrames[iter_uSpriteID].hw_sprites[i] =
                                &pSprites_LOD->pHardwareSprites[v12];
                        }
                    }

                    if (!(pSpriteSFrames[iter_uSpriteID].uFlags & 1)) return;
                    ++iter_uSpriteID;
                }
            }
        }
    }
}

//----- (0044D813) --------------------------------------------------------
int SpriteFrameTable::FastFindSprite(std::string_view pSpriteName) {
    signed int result;  // eax@2

    int searchResult = BinarySearch(pSpriteName);
    if (searchResult < 0)
        result = 0;
    else
        result = this->pSpriteEFrames[searchResult];
    return result;
}

//----- (0044D83A) --------------------------------------------------------
int SpriteFrameTable::BinarySearch(std::string_view pSpriteName) {
    SpriteFrame **result = std::lower_bound(this->pSpritePFrames, this->pSpritePFrames + uNumEFrames, pSpriteName,
        [](SpriteFrame *l, std::string_view r) {
            return iless(l->icon_name, r);
        }
    );

    if (iequals((*result)->icon_name, pSpriteName)) {
        return std::distance(this->pSpritePFrames, result);
    } else {
        return -1;
    }
}

//----- (0044D8D0) --------------------------------------------------------
SpriteFrame *SpriteFrameTable::GetFrame(unsigned int uSpriteID, unsigned int uTime) {
    SpriteFrame *v4 = &pSpriteSFrames[uSpriteID];
    if (~v4->uFlags & 1 || !v4->uAnimLength) return pSpriteSFrames + uSpriteID;


    // uAnimLength / uAnimTime = actual number of frames in sprite
    for (uint t = (uTime / 8) % v4->uAnimLength; t > v4->uAnimTime; ++v4)
        t -= v4->uAnimTime;

    // TODO(pskelton): investigate and fix properly - dragon breath is missing last two frames??
    // quick fix so it doesnt return empty sprite
    while (v4->hw_sprites[0] == NULL) {
        //__debugbreak();
        --v4;
    }

    return v4;

    /*for (v4; v4->uAnimTime <= t; ++v4)

      v6 = (uTime / 8) % v4->uAnimLength;
      //v7 = uSpriteID;
      for ( i = (char *)&v4->uAnimTime; ; i += 60 )
      {
        v9 = *(short *)i;
        if ( v6 <= v9 )
          break;
        v6 -= v9;
        ++v7;
      }
      return &pSpriteSFrames[v7];*/
}

//----- (0044D91F) --------------------------------------------------------
SpriteFrame *SpriteFrameTable::GetFrameBy_x(unsigned int uSpriteID,
                                            signed int a3) {
    SpriteFrame *v3;      // edi@1
    SpriteFrame *v4;      // esi@1
    int16_t v5;           // ax@2
    int v6;               // ecx@3
    int v7;               // edx@3
    unsigned int v8;      // eax@3
    int v9;               // ecx@3
    char *i;              // edx@3
    int v11;              // esi@5
    SpriteFrame *result;  // eax@6

    v3 = this->pSpriteSFrames;
    v4 = &v3[uSpriteID];
    if (v4->uFlags & 1 && (v5 = v4->uAnimLength) != 0) {
        v6 = v5;
        v7 = a3 % v5;
        v8 = uSpriteID;
        v9 = v6 - v7;
        for (i = (char *)&v4->uAnimTime;; i += 60) {
            v11 = *(short *)i;
            if (v9 <= v11) break;
            v9 -= v11;
            ++v8;
        }
        result = &v3[v8];
    } else {
        result = &v3[uSpriteID];
    }
    return result;
}

// new
void SpriteFrameTable::ResetPaletteIndexes() {
    for (int i = 0; i < this->uNumSpriteFrames; i++  ) {
        this->pSpriteSFrames[i].ResetPaletteIndex();
    }
}

void SpriteFrameTable::ToFile() {
    FILE *file = fopen(MakeDataPath("data", "dsft.bin").c_str(), "wb");
    if (file == nullptr) {
        Error("Unable to save dsft.bin!");
    }
    fwrite(&uNumSpriteFrames, 4, 1, file);
    fwrite(&uNumEFrames, 4, 1, file);
    fwrite(pSpriteSFrames, 0x3C, uNumSpriteFrames, file);
    fwrite(pSpriteEFrames, 2, uNumEFrames, file);
    fclose(file);
}

//----- (0044D9D7) --------------------------------------------------------
void SpriteFrameTable::FromFile(const Blob &data_mm6, const Blob &data_mm7, const Blob &data_mm8) {
    uint num_mm6_frames = 0;
    uint num_mm6_eframes = 0;
    if (data_mm6) {
        num_mm6_frames = *(int *)data_mm6.data();
        num_mm6_eframes = *((int *)data_mm6.data() + 1);
    }

    uint num_mm7_frames = 0;
    uint num_mm7_eframes = 0;
    if (data_mm7) {
        num_mm7_frames = *(int *)data_mm7.data();
        num_mm7_eframes = *((int *)data_mm7.data() + 1);
    }

    uint num_mm8_frames = 0;
    uint num_mm8_eframes = 0;
    if (data_mm8) {
        num_mm8_frames = *(int *)data_mm8.data();
        num_mm8_eframes = *((int *)data_mm8.data() + 1);
    }

    this->uNumSpriteFrames =
        num_mm7_frames /*+ num_mm6_frames + num_mm8_frames*/;

    this->pSpriteSFrames = new SpriteFrame[this->uNumSpriteFrames];
    for (unsigned int i = 0; i < this->uNumSpriteFrames; ++i) {
        Deserialize(*((SpriteFrame_MM7 *)((char *)data_mm7.data() + 8) + i), &this->pSpriteSFrames[i]);
    }

    this->uNumEFrames = num_mm7_eframes /*+ num_mm6_eframes + num_mm8_eframes*/;
    this->pSpriteEFrames = (int16_t *)malloc(uNumSpriteFrames * sizeof(short));

    uint mm7_frames_size = num_mm7_frames * sizeof(SpriteFrame_MM7);
    memcpy(pSpriteEFrames, (char *)data_mm7.data() + 8 + mm7_frames_size,
           2 * num_mm7_eframes);

    pSpritePFrames = (SpriteFrame **)malloc(sizeof(void *) * uNumSpriteFrames);

    /*uint mm6_frames_size = num_mm6_frames * sizeof(SpriteFrame_mm6);
    for (uint i = 0; i < num_mm6_frames; ++i)
    {
        memcpy(pSpriteSFrames + num_mm7_frames + i, (char *)data_mm6 + 8 + i *
    sizeof(SpriteFrame_mm6), sizeof(SpriteFrame_mm6));
        pSpriteSFrames[num_mm7_frames + i].uAnimLength = 0;
    }
    memcpy(pSpriteEFrames + num_mm7_frames, (char *)data_mm6 + 8 +
    mm6_frames_size, 2 * num_mm6_eframes);*/

    /*uint mm8_frames_size = num_mm8_frames * sizeof(SpriteFrame);
    memcpy(pSpriteSFrames + num_mm6_frames + num_mm7_frames, (char *)data_mm8 +
    8, mm8_frames_size); memcpy(pSpriteEFrames + num_mm6_frames +
    num_mm7_frames, (char *)data_mm8 + 8 + mm8_frames_size, 2 *
    num_mm8_eframes);*/

    // the original was using num_mmx_frames, but never accessed any element
    // beyond num_mmx_eframes, but boing beyong eframes caused invalid memory
    // accesses
    for (uint i = 0;
         i < num_mm7_eframes /*+ num_mm6_eframes + num_mm8_eframes*/; ++i)
        pSpritePFrames[i] = &pSpriteSFrames[pSpriteEFrames[i]];
}

SpriteFrame *LevelDecorationChangeSeason(const DecorationDesc *desc, int t, int month) {
    switch (month/*pParty->uCurrentMonth*/) {
        // case 531 (tree60), 536 (tree65), 537 (tree66) have no autumn/winter
        // sprites
        case 11:
        case 0:
        case 1:  // winter
        {
            switch (desc->uSpriteID) {
                // case 468:           //bush02    grows on swamps, which are
                // evergreeen actually
                case 548:  // flower10
                case 547:  // flower09
                case 541:  // flower03
                case 539:  // flower01
                    return nullptr;

                case 483:  // tree01
                case 486:  // tree04
                case 492:  // tree10
                {
                    pSpriteFrameTable->InitializeSprite(desc->uSpriteID + 2);
                    return pSpriteFrameTable->GetFrame(desc->uSpriteID + 2, t);
                }

                default:
                    return pSpriteFrameTable->GetFrame(desc->uSpriteID, t);
            }
        }

        case 2:
        case 3:
        case 4:  // spring
        {
            // switch (desc->uSpriteID) {}
            return pSpriteFrameTable->GetFrame(desc->uSpriteID, t);
        }

        case 8:
        case 9:
        case 10:  // autumn
        {
            switch (desc->uSpriteID) {
                // case 468: //bush02    grows on swamps, which are evergreeen
                // actually
                case 548:  // flower10
                case 547:  // flower09
                case 541:  // flower03
                case 539:  // flower01
                    return nullptr;

                case 483:  // tree01
                case 486:  // tree04
                case 492:  // tree10
                {
                    pSpriteFrameTable->InitializeSprite(desc->uSpriteID + 1);
                    return pSpriteFrameTable->GetFrame(desc->uSpriteID + 1, t);
                }

                default:
                    return pSpriteFrameTable->GetFrame(desc->uSpriteID, t);
            }
        } break;

        case 5:
        case 6:
        case 7:  // summer
                 // all green by default
        {
            return pSpriteFrameTable->GetFrame(desc->uSpriteID, t);
        }

        default:
            assert(/*pParty->uCurrentMonth*/month >= 0 && /*pParty->uCurrentMonth*/month < 12);
    }
    logger->Warning("No sprite returned - LevelDecorationChangeSeason!");
    return nullptr;
}

int SpriteFrame::GetPaletteIndex() {
    if (!engine->config->graphics.HWLSprites.Get()) {
        if (this->uPaletteIndex == 0)
            this->uPaletteIndex = pPaletteManager->LoadPalette(this->uPaletteID);
        return this->uPaletteIndex;
    }
    return 0;
}

void SpriteFrame::ResetPaletteIndex(int index) {
    this->uPaletteIndex = index;
}
