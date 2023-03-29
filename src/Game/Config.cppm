export module Game.Config;

import <cstdint>;

// the default values are set according to
//  https://github.com/ClitherProject/Slither.io-Protocol/blob/master/Protocol.md#packet-a-initial-setup
//   and
//  https://github.com/sitano/slitherss/blob/master/src/game/config.h
export struct GameConfig {
    uint16_t worldRadius = 16384;
    uint16_t maxBodyParts = 411;
    uint16_t sectorSize = 300;
    uint16_t sectorCountEdge = static_cast<uint16_t>(2 * worldRadius / sectorSize);
    uint16_t foodDensity = 10;
    uint16_t targetSnakeCount = 100;
    double speedAngularDelta = 4.8;
    double speedAngularBase = 0.033;
};
