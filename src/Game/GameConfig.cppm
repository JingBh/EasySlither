export module Game.GameConfig;

import <cstdint>;

// the default values are set according to
//  https://github.com/ClitherProject/Slither.io-Protocol/blob/master/Protocol.md#packet-a-initial-setup
//   and
//  https://github.com/sitano/slitherss/blob/master/src/game/config.h
export struct GameConfig {
    uint16_t worldRadius = 16384;
    uint16_t maxBodyParts = 411;
    uint16_t sectorSize = 300;
    uint8_t sectorCountEdge = static_cast<uint8_t>(2 * worldRadius / sectorSize);
    uint16_t foodDensity = 10;
    uint16_t targetSnakeCount = 200;
};
