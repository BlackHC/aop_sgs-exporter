// Copyright 2002-2004 Frozenbyte Ltd.

// from 1100

#define GS_CMD_BASE 1100

GS_CMD(0, GS_CMD_GETPOSITIONLIGHTAMOUNT, "getPositionLightAmount", NONE)
GS_CMD(1, GS_CMD_GETPOSITIONDYNAMICLIGHTAMOUNT, "getPositionDynamicLightAmount", NONE)
GS_CMD(2, GS_CMD_GETPOSITIONSTATICLIGHTAMOUNT, "getPositionStaticLightAmount", NONE)
GS_CMD(3, GS_CMD_GETPOSITIONDYNAMICLIGHTORIGINUNIT, "getPositionDynamicLightOriginUnit", NONE)
GS_CMD(4, GS_CMD_GETPOSITIONMAXSTATICLIGHTAMOUNT, "getPositionMaxStaticLightAmount", NONE)
GS_CMD(5, GS_CMD_SWAPTEXTURETOSTRINGVALUE, "swapTextureToStringValue", STRING)
GS_CMD(6, GS_CMD_SETAMBIENTLIGHT, "setAmbientLight", INT)
GS_CMD(7, GS_CMD_SETAMBIENTLIGHTTOVALUE, "setAmbientLightToValue", NONE)
GS_CMD(8, GS_CMD_SETAMBIENTLIGHTRED, "setAmbientLightRed", INT)
GS_CMD(9, GS_CMD_SETAMBIENTLIGHTREDTOVALUE, "setAmbientLightRedToValue", NONE)
GS_CMD(10, GS_CMD_SETAMBIENTLIGHTGREEN, "setAmbientLightGreen", INT)
GS_CMD(11, GS_CMD_SETAMBIENTLIGHTGREENTOVALUE, "setAmbientLightGreenToValue", NONE)
GS_CMD(12, GS_CMD_SETAMBIENTLIGHTBLUE, "setAmbientLightBlue", INT)
GS_CMD(13, GS_CMD_SETAMBIENTLIGHTBLUETOVALUE, "setAmbientLightBlueToValue", NONE)

GS_CMD(14, GS_CMD_ADDLIGHT, "addLight", NONE)
GS_CMD(15, GS_CMD_SETLIGHTANIMATIONTYPE, "setLightAnimationType", STRING)
GS_CMD(16, GS_CMD_SETLIGHTCOLORRED, "setLightColorRed", FLOAT)
GS_CMD(17, GS_CMD_SETLIGHTCOLORGREEN, "setLightColorGreen", FLOAT)
GS_CMD(18, GS_CMD_SETLIGHTCOLORBLUE, "setLightColorBlue", FLOAT)
GS_CMD(19, GS_CMD_SETLIGHTSHADOWS, "setLightShadows", INT)
GS_CMD(20, GS_CMD_SETLIGHTGROUP, "setLightGroup", INT)
GS_CMD(21, GS_CMD_ENABLELIGHTGROUP, "enableLightGroup", INT)
GS_CMD(22, GS_CMD_DISABLELIGHTGROUP, "disableLightGroup", INT)
GS_CMD(23, GS_CMD_SETLIGHTMAPBRIGHTNESS, "setLightmapBrightness", INT)
GS_CMD(24, GS_CMD_SETCOLORMAPBRIGHTNESS, "setColormapBrightness", INT)
GS_CMD(25, GS_CMD_SETLIGHTTYPE, "setLightType", STRING)
GS_CMD(26, GS_CMD_SETLIGHTPRIORITY, "setLightPriority", INT)

GS_CMD(27, GS_CMD_SETCOLORMAPBRIGHTNESSRED, "setColormapBrightnessRed", INT)
GS_CMD(28, GS_CMD_SETCOLORMAPBRIGHTNESSGREEN, "setColormapBrightnessGreen", INT)
GS_CMD(29, GS_CMD_SETCOLORMAPBRIGHTNESSBLUE, "setColormapBrightnessBlue", INT)
GS_CMD(30, GS_CMD_SETLIGHTMAPBRIGHTNESSRED, "setLightmapBrightnessRed", INT)
GS_CMD(31, GS_CMD_SETLIGHTMAPBRIGHTNESSGREEN, "setLightmapBrightnessGreen", INT)
GS_CMD(32, GS_CMD_SETLIGHTMAPBRIGHTNESSBLUE, "setLightmapBrightnessBlue", INT)

GS_CMD(33, GS_CMD_SETLIGHTMAPBRIGHTNESSTOVALUE, "setLightmapBrightnessToValue", NONE)
GS_CMD(34, GS_CMD_SETCOLORMAPBRIGHTNESSTOVALUE, "setColormapBrightnessToValue", NONE)
GS_CMD(35, GS_CMD_SETCOLORMAPBRIGHTNESSREDTOVALUE, "setColormapBrightnessRedToValue", NONE)
GS_CMD(36, GS_CMD_SETCOLORMAPBRIGHTNESSGREENTOVALUE, "setColormapBrightnessGreenToValue", NONE)
GS_CMD(37, GS_CMD_SETCOLORMAPBRIGHTNESSBLUETOVALUE, "setColormapBrightnessBlueToValue", NONE)
GS_CMD(38, GS_CMD_SETLIGHTMAPBRIGHTNESSREDTOVALUE, "setLightmapBrightnessRedToValue", NONE)
GS_CMD(39, GS_CMD_SETLIGHTMAPBRIGHTNESSGREENTOVALUE, "setLightmapBrightnessGreenToValue", NONE)
GS_CMD(40, GS_CMD_SETLIGHTMAPBRIGHTNESSBLUETOVALUE, "setLightmapBrightnessBlueToValue", NONE)

GS_CMD(41, GS_CMD_SETLIGHTINGSPOTCULLRANGE, "setLightingSpotCullRange", INT)
GS_CMD(42, GS_CMD_SETLIGHTINGSPOTFADEOUTRANGE, "setLightingSpotFadeoutRange", INT)

GS_CMD(43, GS_CMD_SETLIGHTANGLEY, "setLightAngleY", FLOAT)
GS_CMD(44, GS_CMD_SETLIGHTANGLEX, "setLightAngleX", FLOAT)
GS_CMD(45, GS_CMD_SETLIGHTRANGE, "setLightRange", FLOAT)
GS_CMD(46, GS_CMD_SETLIGHTFOV, "setLightFOV", FLOAT)
GS_CMD(47, GS_CMD_SETLIGHTHEIGHT, "setLightHeight", FLOAT)
GS_CMD(48, GS_CMD_SETLIGHTBLINK, "setLightBlink", INT)
GS_CMD(49, GS_CMD_SETLIGHTBLINKTIME, "setLightBlinkTime", INT)
GS_CMD(50, GS_CMD_SETLIGHTROTATE, "setLightRotate", INT)
GS_CMD(51, GS_CMD_SETLIGHTROTATETIME, "setLightRotateTime", INT)
GS_CMD(52, GS_CMD_SETLIGHTFADE, "setLightFade", INT)
GS_CMD(53, GS_CMD_SETLIGHTFADETIME, "setLightFadeTime", INT)
GS_CMD(54, GS_CMD_SETLIGHTCONE, "setLightCone", FLOAT)
GS_CMD(55, GS_CMD_SETLIGHTRESERVED1, "setLightReserved1", NONE)
GS_CMD(56, GS_CMD_SETLIGHTTEXTURE, "setLightTexture", STRING)
GS_CMD(57, GS_CMD_SETLIGHTCONETEXTURE, "setLightConeTexture", STRING)
GS_CMD(58, GS_CMD_SETLIGHTMODEL, "setLightModel", STRING)
GS_CMD(59, GS_CMD_SETLIGHTMINPLANEX, "setLightMinPlaneX", FLOAT)
GS_CMD(60, GS_CMD_SETLIGHTMINPLANEY, "setLightMinPlaneY", FLOAT)
GS_CMD(61, GS_CMD_SETLIGHTMAXPLANEX, "setLightMaxPlaneX", FLOAT)
GS_CMD(62, GS_CMD_SETLIGHTMAXPLANEY, "setLightMaxPlaneY", FLOAT)
GS_CMD(63, GS_CMD_SETLIGHTSTRENGTH, "setLightStrength", FLOAT)

GS_CMD(64, GS_CMD_SETLIGHTDISABLEOBJECTRENDERING, "setLightDisableObjectRendering", INT)
GS_CMD(65, GS_CMD_SETLIGHTSOURCEHEIGHT, "setLightSourceHeight", FLOAT)

GS_CMD(66, GS_CMD_SETBLINKINGLIGHT1, "setBlinkingLight1", INT)
GS_CMD(67, GS_CMD_SETBLINKINGLIGHT1TOVALUE, "setBlinkingLight1ToValue", NONE)
GS_CMD(68, GS_CMD_SETBLINKINGLIGHT1RED, "setBlinkingLight1Red", INT)
GS_CMD(69, GS_CMD_SETBLINKINGLIGHT1REDTOVALUE, "setBlinkingLight1RedToValue", NONE)
GS_CMD(70, GS_CMD_SETBLINKINGLIGHT1GREEN, "setBlinkingLight1Green", INT)
GS_CMD(71, GS_CMD_SETBLINKINGLIGHT1GREENTOVALUE, "setBlinkingLight1GreenToValue", NONE)
GS_CMD(72, GS_CMD_SETBLINKINGLIGHT1BLUE, "setBlinkingLight1Blue", INT)
GS_CMD(73, GS_CMD_SETBLINKINGLIGHT1BLUETOVALUE, "setBlinkingLight1BlueToValue", NONE)

GS_CMD(74, GS_CMD_SETBLINKINGLIGHT2, "setBlinkingLight2", INT)
GS_CMD(75, GS_CMD_SETBLINKINGLIGHT2TOVALUE, "setBlinkingLight2ToValue", NONE)
GS_CMD(76, GS_CMD_SETBLINKINGLIGHT2RED, "setBlinkingLight2Red", INT)
GS_CMD(77, GS_CMD_SETBLINKINGLIGHT2REDTOVALUE, "setBlinkingLight2RedToValue", NONE)
GS_CMD(78, GS_CMD_SETBLINKINGLIGHT2GREEN, "setBlinkingLight2Green", INT)
GS_CMD(79, GS_CMD_SETBLINKINGLIGHT2GREENTOVALUE, "setBlinkingLight2GreenToValue", NONE)
GS_CMD(80, GS_CMD_SETBLINKINGLIGHT2BLUE, "setBlinkingLight2Blue", INT)
GS_CMD(81, GS_CMD_SETBLINKINGLIGHT2BLUETOVALUE, "setBlinkingLight2BlueToValue", NONE)

GS_CMD(82, GS_CMD_SETBLINKINGLIGHTSTRENGTH, "setBlinkingLightStrength", INT)
GS_CMD(83, GS_CMD_SETBLINKINGLIGHTSTRENGTHTOVALUE, "setBlinkingLightStrengthToValue", NONE)

GS_CMD(84, GS_CMD_SETBLINKINGLIGHTFREQUENCY, "setBlinkingLightFrequency", INT)
GS_CMD(85, GS_CMD_SETBLINKINGLIGHTFREQUENCYTOVALUE, "setBlinkingLightFrequencyToValue", NONE)

GS_CMD(86, GS_CMD_SETBLINKINGLIGHTRANDOM, "setBlinkingLightRandom", INT)
GS_CMD(87, GS_CMD_SETBLINKINGLIGHTRANDOMTOVALUE, "setBlinkingLightRandomToValue", NONE)

GS_CMD(88, GS_CMD_ENABLEBLINKINGLIGHT, "enableBlinkingLight", NONE)
GS_CMD(89, GS_CMD_DISABLEBLINKINGLIGHT, "disableBlinkingLight", NONE)

GS_CMD(90, GS_CMD_SETBLINKINGLIGHTPAUSETIME, "setBlinkingLightPauseTime", INT)
GS_CMD(91, GS_CMD_SETBLINKINGLIGHTPAUSETIMETOVALUE, "setBlinkingLightPauseTimeToValue", NONE)
GS_CMD(92, GS_CMD_SETBLINKINGLIGHTPAUSEVARIATION, "setBlinkingLightPauseVariation", INT)
GS_CMD(93, GS_CMD_SETBLINKINGLIGHTPAUSEVARIATIONTOVALUE, "setBlinkingLightPauseVariationToValue", NONE)

GS_CMD(94, GS_CMD_SETBLINKINGLIGHTRANDOMTIME, "setBlinkingLightRandomTime", INT)
GS_CMD(95, GS_CMD_SETBLINKINGLIGHTTYPE, "setBlinkingLightType", STRING)

GS_CMD(96, GS_CMD_SETBLINKINGLIGHTDEFAULT, "setBlinkingLightDefault", INT)
GS_CMD(97, GS_CMD_SETBLINKINGLIGHTDEFAULTTOVALUE, "setBlinkingLightDefaultToValue", NONE)

GS_CMD(98, GS_CMD_SETOUTDOORLIGHTMAPBRIGHTNESS, "setOutdoorLightmapBrightness", INT)
GS_CMD(99, GS_CMD_SETOUTDOORLIGHTMAPBRIGHTNESSTOVALUE, "setOutdoorLightmapBrightnessToValue", NONE)

GS_CMD(100, GS_CMD_SETLIGHTPOINTLIGHT, "setLightPointLight", INT)

GS_CMD(101, GS_CMD_SETFORCEAMBIENTLIGHT, "setForceAmbientLight", INT)
GS_CMD(102, GS_CMD_SETFORCEAMBIENTLIGHTTOVALUE, "setForceAmbientLightToValue", NONE)

GS_CMD_SIMPLE(103, setOutdoorBlinkingLight1, INT)
GS_CMD_SIMPLE(104, setOutdoorBlinkingLight1ToValue, NONE)
GS_CMD_SIMPLE(105, setOutdoorBlinkingLight1Red, INT)
GS_CMD_SIMPLE(106, setOutdoorBlinkingLight1RedToValue, NONE)
GS_CMD_SIMPLE(107, setOutdoorBlinkingLight1Green, INT)
GS_CMD_SIMPLE(108, setOutdoorBlinkingLight1GreenToValue, NONE)
GS_CMD_SIMPLE(109, setOutdoorBlinkingLight1Blue, INT)
GS_CMD_SIMPLE(110, setOutdoorBlinkingLight1BlueToValue, NONE)

GS_CMD_SIMPLE(111, setOutdoorBlinkingLight2, INT)
GS_CMD_SIMPLE(112, setOutdoorBlinkingLight2ToValue, NONE)
GS_CMD_SIMPLE(113, setOutdoorBlinkingLight2Red, INT)
GS_CMD_SIMPLE(114, setOutdoorBlinkingLight2RedToValue, NONE)
GS_CMD_SIMPLE(115, setOutdoorBlinkingLight2Green, INT)
GS_CMD_SIMPLE(116, setOutdoorBlinkingLight2GreenToValue, NONE)
GS_CMD_SIMPLE(117, setOutdoorBlinkingLight2Blue, INT)
GS_CMD_SIMPLE(118, setOutdoorBlinkingLight2BlueToValue, NONE)

GS_CMD_SIMPLE(119, setOutdoorBlinkingLightStrength, INT)
GS_CMD_SIMPLE(120, setOutdoorBlinkingLightStrengthToValue, NONE)

GS_CMD_SIMPLE(121, setOutdoorBlinkingLightFrequency, INT)
GS_CMD_SIMPLE(122, setOutdoorBlinkingLightFrequencyToValue, NONE)

GS_CMD_SIMPLE(123, setOutdoorBlinkingLightRandom, INT)
GS_CMD_SIMPLE(124, setOutdoorBlinkingLightRandomToValue, NONE)

GS_CMD_SIMPLE(125, enableOutdoorBlinkingLight, NONE)
GS_CMD_SIMPLE(126, disableOutdoorBlinkingLight, NONE)

GS_CMD_SIMPLE(127, setOutdoorBlinkingLightPauseTime, INT)
GS_CMD_SIMPLE(128, setOutdoorBlinkingLightPauseTimeToValue, NONE)
GS_CMD_SIMPLE(129, setOutdoorBlinkingLightPauseVariation, INT)
GS_CMD_SIMPLE(130, setOutdoorBlinkingLightPauseVariationToValue, NONE)

GS_CMD_SIMPLE(131, setOutdoorBlinkingLightRandomTime, INT)
GS_CMD_SIMPLE(132, setOutdoorBlinkingLightType, STRING)

GS_CMD_SIMPLE(133, setOutdoorBlinkingLightDefault, INT)
GS_CMD_SIMPLE(134, setOutdoorBlinkingLightDefaultToValue, NONE)
GS_CMD_SIMPLE(135, addBuildingLight, NONE)
GS_CMD_SIMPLE(136, setOutdoorLightmapBrightnessNearPositionToValue, INT)
GS_CMD_SIMPLE(137, setGlowFadeFactor, FLOAT)
GS_CMD_SIMPLE(138, setGlowTransparencyFactor, FLOAT)
GS_CMD_SIMPLE(139, setGlowAdditiveFactor, FLOAT)

GS_CMD_SIMPLE(140, applyLightProperties, NONE)
GS_CMD_SIMPLE(141, setLightIdString, STRING)
GS_CMD_SIMPLE(142, getLightIdString, NONE)
GS_CMD_SIMPLE(143, setUnifiedHandleByLightIdString, STRING)
GS_CMD_SIMPLE(144, updateLight, NONE)

GS_CMD_SIMPLE(145, addDynamicLight, NONE)
GS_CMD_SIMPLE(146, applyDynamicLightProperties, NONE)
GS_CMD_SIMPLE(147, getDynamicLightProperties, NONE)

GS_CMD_SIMPLE(148, getLightProperties, NONE)

GS_CMD_SIMPLE(149, changeLightPosition, NONE)
GS_CMD_SIMPLE(150, changeLightRadiusToValue, NONE)
GS_CMD_SIMPLE(151, changeLightRadiusToFloatValue, NONE)
GS_CMD_SIMPLE(152, changeLightColor, COLOR_RGB)
GS_CMD_SIMPLE(153, deleteLight, NONE)
GS_CMD_SIMPLE(154, deleteAllLights, NONE)

GS_CMD_SIMPLE(155, getFirstLightUnifiedHandle, NONE)
GS_CMD_SIMPLE(156, getNextLightUnifiedHandle, NONE)

GS_CMD_SIMPLE(157, updateAllTerrainObjectLighting, NONE)

// new spotlight type handling (not used in legacy)
GS_CMD_SIMPLE(158, spotTypeAdd, STRING)
GS_CMD_SIMPLE(159, spotTypeSetTexture, STRING)
GS_CMD_SIMPLE(160, spotTypeSetConeTexture, STRING)
GS_CMD_SIMPLE(161, spotTypeSetFOV, FLOAT)
GS_CMD_SIMPLE(162, spotTypeSetConeFOV, FLOAT)
GS_CMD_SIMPLE(163, spotTypeSetRange, FLOAT)
GS_CMD_SIMPLE(164, spotTypeSetType, STRING)
GS_CMD_SIMPLE(165, spotTypeSetConeMultiplier, FLOAT)
GS_CMD_SIMPLE(166, spotTypeSetModel, STRING)
GS_CMD_SIMPLE(167, spotTypeSetShadows, INT)
GS_CMD_SIMPLE(168, spotTypeSetFade, INT)
GS_CMD_SIMPLE(169, spotTypeSetNoShadowFromOrigin, INT)
GS_CMD_SIMPLE(170, spotTypeSetFakelight, INT)
GS_CMD_SIMPLE(171, spotTypeSetFadeRange, FLOAT)
GS_CMD_SIMPLE(172, spotTypeSetScissor, INT)
GS_CMD_SIMPLE(173, spotTypeSetDirection, VECTOR_XYZ)
GS_CMD_SIMPLE(174, spotTypeSetColorMultiplier, COLOR_RGB)
GS_CMD_SIMPLE(175, spotTypeSetFakelightColor, COLOR_RGB)
GS_CMD_SIMPLE(176, spotTypeSetFlashType, STRING)
GS_CMD_SIMPLE(177, spotTypeSetLowDetailVersion, STRING)
GS_CMD_SIMPLE(178, spotTypeSetPositionOffset, STRING)
GS_CMD_SIMPLE(179, spotTypeAddDone, NONE)
// end of new spotlight type handling

GS_CMD_SIMPLE(180, setLightLightingModelType, INT)
GS_CMD_SIMPLE(181, setLightLightingModelFade, INT)

GS_CMD_SIMPLE(182, setLightRotateRange, FLOAT)

GS_CMD_SIMPLE(183, findClosestLight, NONE)
GS_CMD_SIMPLE(184, findClosestDetachedLight, NONE)
GS_CMD_SIMPLE(185, attachLightToValueHandle, NONE)

#undef GS_CMD_BASE

// up to 1299

