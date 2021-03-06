// Copyright 2002-2004 Frozenbyte Ltd.

// from 1900

#define GS_CMD_BASE 1900

GS_CMD(0, GS_CMD_SETSTRING, "setString", STRING)
GS_CMD(1, GS_CMD_APPENDSTRING, "appendString", STRING)
GS_CMD(2, GS_CMD_PREPENDSTRING, "prependString", STRING)
GS_CMD(3, GS_CMD_SUBSTRINGFROMSTART, "substringFromStart", INT)
GS_CMD(4, GS_CMD_SUBSTRINGFROMEND, "substringFromEnd", INT)
GS_CMD(5, GS_CMD_VALUETOSTRING, "valueToString", NONE)
GS_CMD(6, GS_CMD_VALUETOCLOCKSTRING, "valueToClockString", STRING)
GS_CMD(7, GS_CMD_STRINGLENGTH, "stringLength", NONE)
GS_CMD(8, GS_CMD_ISNULLSTRING, "isNullString", NONE)
GS_CMD(9, GS_CMD_SETNULLSTRING, "setNullString", NONE)
GS_CMD(10, GS_CMD_PRINTSTRINGVALUE, "printStringValue", NONE)
GS_CMD(11, GS_CMD_SUBSTRINGFROMSTARTBYVALUE, "substringFromStartByValue", NONE)
GS_CMD(12, GS_CMD_SUBSTRINGFROMENDBYVALUE, "substringFromEndByValue", NONE)
GS_CMD(13, GS_CMD_STRINGEQUALS, "stringEquals", STRING)
GS_CMD(14, GS_CMD_APPENDVALUETOSTRING, "appendValueToString", NONE)
GS_CMD(15, GS_CMD_APPENDNEXTLINESTRING, "appendNextLineString", STRING)
GS_CMD(16, GS_CMD_APPENDSTRINGLINEFEED, "appendStringLineFeed", NONE)
GS_CMD_SIMPLE(17, stringToValue, NONE)
GS_CMD_SIMPLE(18, stringCutAtFirstLine, NONE)

#undef GS_CMD_BASE

// up to 1999
