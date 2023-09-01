/*
* ProgressBar TextDraw (PlayerBar)
* 
* Time: 26/07/2023 (Vietnam)
* Author: Ausil (#aujiz)
*
* File name: progresstd.inc
*
* progresstd.inc là một thư viện dùng lập trình viên có thể tạo một thanh
* trạng thái cho người chơi trên máy chủ, khác với progress2.inc (Southclaws)
* progresstd.inc không quá cầu kì và đầy đủ tính năng như vậy, những ưu điểm
* thư viện này sẽ cho bạn khả năng tạo progressBar đơn giản mà hiện đại và đẹp
* mắt.
*
* progresstd.inc is a library where programmers can create a bar
* status for players on the server, different from progress2.inc (Southclaws)
* progresstd.inc is not so complicated and full of features, advantages
* this library will give you the ability to create simple yet modern and beautiful progressBar
* eye.
*
*/

#if defined _progresstd_included
	#endinput
#endif
#define _progresstd_included

#if !defined _INC_open_mp
	#error The <open.mp> library must be called first
#endif

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

#define MAX_PLAYER_BARS         (_:30)
#define INVALID_PLAYER_BAR      (PlayerBar:-1)


/*
*
* Enum
*
*/
enum E_BAR_TD
{
    PlayerText:barMain,
    PlayerText:barFill
};

enum E_BAR_INFO
{
    Float:barX,
    Float:barY,
    Float:barWidth,
    Float:barHeight,
    Float:barMax,
    Float:barValue,
    barMainColour,
    barFillColour,
    bool:barShow
};


/*
*
* Variables
*
*/
static barTextDraw[MAX_PLAYERS][MAX_PLAYER_BARS][E_BAR_TD];

new pBarInfo[MAX_PLAYERS][MAX_PLAYER_BARS][E_BAR_INFO];
new Iterator:barIterator[MAX_PLAYERS]<MAX_PLAYER_BARS>;


/*
*
* bool: PlayerBar_Show(playerid, PlayerBar: barid)
*
* Show a progressbar for player
*/
stock bool: PlayerBar_Show(playerid, PlayerBar: barid)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;

    pBarInfo[playerid][_:barid][barShow] = true;
    PlayerTextDrawShow(playerid, barTextDraw[playerid][_:barid][barMain]);
    PlayerTextDrawShow(playerid, barTextDraw[playerid][_:barid][barFill]);

    return true;
}

/*
*
* bool: PlayerBar_Hide(playerid, PlayerBar: barid)
*
* Hide a progressbar for player
*/
stock bool: PlayerBar_Hide(playerid, PlayerBar: barid)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;
        
    pBarInfo[playerid][_:barid][barShow] = false;
    PlayerTextDrawHide(playerid, barTextDraw[playerid][_:barid][barMain]);
    PlayerTextDrawHide(playerid, barTextDraw[playerid][_:barid][barFill]);
    return true;
}

/*
*
* ?
* ?
*
 */
stock bool: PlayerBar_Render(playerid, PlayerBar: barid)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;

    PlayerTextDrawDestroy(playerid, barTextDraw[playerid][_:barid][barMain]);
    PlayerTextDrawDestroy(playerid, barTextDraw[playerid][_:barid][barFill]);

    barTextDraw[playerid][_:barid][barMain] = CreatePlayerTextDraw(
        playerid, 
        pBarInfo[playerid][_:barid][barX],
        pBarInfo[playerid][_:barid][barY], 
        "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, barTextDraw[playerid][_:barid][barMain], pBarInfo[playerid][_:barid][barWidth], pBarInfo[playerid][_:barid][barHeight]);
    PlayerTextDrawAlignment(playerid, barTextDraw[playerid][_:barid][barMain], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, barTextDraw[playerid][_:barid][barMain], pBarInfo[playerid][_:barid][barMainColour]);
    PlayerTextDrawSetShadow(playerid, barTextDraw[playerid][_:barid][barMain], 0);
    PlayerTextDrawSetOutline(playerid, barTextDraw[playerid][_:barid][barMain], 0);
    PlayerTextDrawBackgroundColour(playerid, barTextDraw[playerid][_:barid][barMain], 255);
    PlayerTextDrawFont(playerid, barTextDraw[playerid][_:barid][barMain], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, barTextDraw[playerid][_:barid][barMain], true);

    barTextDraw[playerid][_:barid][barFill] = CreatePlayerTextDraw(
        playerid, 
        pBarInfo[playerid][_:barid][barX], 
        pBarInfo[playerid][_:barid][barY], 
        "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, barTextDraw[playerid][_:barid][barFill], pBarInfo[playerid][_:barid][barWidth], pBarInfo[playerid][_:barid][barHeight]);
    PlayerTextDrawAlignment(playerid, barTextDraw[playerid][_:barid][barFill], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, barTextDraw[playerid][_:barid][barFill], pBarInfo[playerid][_:barid][barFillColour]);
    PlayerTextDrawSetShadow(playerid, barTextDraw[playerid][_:barid][barFill], 0);
    PlayerTextDrawSetOutline(playerid, barTextDraw[playerid][_:barid][barFill], 0);
    PlayerTextDrawBackgroundColour(playerid, barTextDraw[playerid][_:barid][barFill], 255);
    PlayerTextDrawFont(playerid, barTextDraw[playerid][_:barid][barFill], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, barTextDraw[playerid][_:barid][barFill], true);

    if(pBarInfo[playerid][_:barid][barShow] == true)
        PlayerBar_Show(playerid, PlayerBar:barid);

    return true;
}

/*
*
* PlayerBar_Create(playerid, colour, Float:x, Float:y, Float:width, Float:height, Float:maxValue = 100.0, bool:show = false)
*
* Create a progressbar 
*/
stock PlayerBar: PlayerBar_Create(playerid, mainColour, fillColour, Float:x, Float:y, Float:width, Float:height, Float:maxValue = 100.0, bool:show = false)
{
    new barid = Iter_Free(barIterator[playerid]);
    if(barid == ITER_NONE) {
        print("PlayerBar has reached its limit");
        return INVALID_PLAYER_BAR;
    }
    
    if(!IsPlayerConnected(playerid)) {
        printf("This player is not connected yet (playerid: %d)", playerid);
        return INVALID_PLAYER_BAR;
    }

    Iter_Add(barIterator[playerid], barid);
    barTextDraw[playerid][_:barid][barMain] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
    barTextDraw[playerid][_:barid][barFill] = PlayerText:INVALID_PLAYER_TEXT_DRAW;

    pBarInfo[playerid][barid][barShow] = show;
    pBarInfo[playerid][barid][barMainColour] = mainColour;
    pBarInfo[playerid][barid][barFillColour] = fillColour;
    pBarInfo[playerid][barid][barX] = x;
    pBarInfo[playerid][barid][barY] = y;
    pBarInfo[playerid][barid][barWidth] = width;
    pBarInfo[playerid][barid][barHeight] = height;
    pBarInfo[playerid][barid][barMax] = maxValue;
    pBarInfo[playerid][barid][barValue] = maxValue;

    PlayerBar_Render(playerid, PlayerBar:barid);

    return PlayerBar: barid;
}

/*
*
* bool: PlayerBar_Destroy(playerid, PlayerBar: barid)
*
* Destroy a progressbar 
*/
stock bool: PlayerBar_Destroy(playerid, PlayerBar: barid)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;

    PlayerTextDrawDestroy(playerid, barTextDraw[playerid][_:barid][barMain]);
    PlayerTextDrawDestroy(playerid, barTextDraw[playerid][_:barid][barFill]);

    pBarInfo[playerid][_:barid][barShow] = false;
    pBarInfo[playerid][_:barid][barMainColour] = 0x00000000;
    pBarInfo[playerid][_:barid][barFillColour] = 0x00000000;
    pBarInfo[playerid][_:barid][barX] = 0.0;
    pBarInfo[playerid][_:barid][barY] = 0.0;
    pBarInfo[playerid][_:barid][barWidth] = 0.0;
    pBarInfo[playerid][_:barid][barHeight] = 0.0;
    pBarInfo[playerid][_:barid][barMax] = 0.0;
    pBarInfo[playerid][_:barid][barValue] = 0.0;

    Iter_Remove(barIterator[playerid], _:barid);
    return true;
}

/*
*
* bool: PlayerBar_SetValue(playerid, PlayerBar: barid, Float:value)
*
* Set current value for progressbar
*/
stock bool: PlayerBar_SetValue(playerid, PlayerBar: barid, Float:value)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;

    if(value > pBarInfo[playerid][_:barid][barMax])
        return false;

    new 
        Float:progress = (pBarInfo[playerid][_:barid][barWidth] / pBarInfo[playerid][_:barid][barMax]) * value;

    pBarInfo[playerid][_:barid][barValue] = value;

    PlayerTextDrawTextSize(playerid, barTextDraw[playerid][_:barid][barFill], progress, pBarInfo[playerid][_:barid][barHeight]);
    return true;
}

/*
*
* bool: PlayerBar_SetValue(playerid, PlayerBar: barid, Float:value)
*
* Set max value for progressbar
*/
stock bool: PlayerBar_SetMaxValue(playerid, PlayerBar: barid, Float:maxValue)
{
    if(!IsValidPlayerBar(playerid, barid))
        return false;

    pBarInfo[playerid][_:barid][barMax] = maxValue;

    new 
        Float:progress = 
        (pBarInfo[playerid][_:barid][barWidth] / pBarInfo[playerid][_:barid][barMax]) * pBarInfo[playerid][_:barid][barValue];

    PlayerTextDrawTextSize(playerid, barTextDraw[playerid][_:barid][barFill], progress, pBarInfo[playerid][_:barid][barHeight]);
    return true;
}

/*
*
* IsValidPlayerBar(playerid, PlayerBar: barid)
*
* Check progressbar exists
*/
stock bool: IsValidPlayerBar(playerid, PlayerBar: barid)
{
    return Iter_Contains(barIterator[playerid], _:barid);
}

/*=.=.=.=.=.=.=.=.= Callbacks =.=.=.=.=.=.=.=.=*/
hook OnPlayerDisconnect(playerid, reason)
{
    foreach(new i: barIterator[playerid])
    {
        PlayerBar_Destroy(playerid, PlayerBar:i);
    }
    return 1;
}
