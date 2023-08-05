#include "open.mp"
#include "progress-td"


new PlayerBar: BarTest[MAX_PLAYERS];

public OnGameModeInit()
{
    print("progress-td.inc by Aujiz");
    return 1;
}

public OnGameModeExit()
{
    return 1;
}

public OnPlayerConnect(playerid)
{
    BarTest[playerid] = PlayerBar_Create(playerid, 0x804400FF, 0xFF941AFF, 507.000, 428.000, 113.000, 7.000);
    return 1;
}