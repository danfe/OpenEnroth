#pragma once

namespace Application {
    class GameConfig;

    bool ParseGameOptions(int argc, char **argv, GameConfig *config);
}
