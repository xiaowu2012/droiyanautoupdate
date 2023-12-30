// AutoUpdateClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoUpdateClient.h"
#include "AutoUpdateClientDlg.h"
#include "Define.h"
#include "..\zlib\zconf.h"
#include "..\zlib\zlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define PUBLIC_TIMER	(WM_USER + 1004)
#define UM_TESDOWN		(WM_USER + 1005)		//测试用
#define	UM_TIMEOUT		(WM_USER + 1006)
#define UM_ANIMATION	(WM_USER + 1007)		


AniPointData g_AniFrameData[] = 
{
	{0, 1},		//0
	{1,2},		//1
	{3,4},		//2
	{7,6},		//3
	{13,8},		//4
	{21,10},	//5
	{31,12},	//6
	{43,13},	//7
	{56,16},	//8
	{72,17},	//9
	{89,19},	//10
	{108,22},	//11
	{130,24},	//12
	{154,26},	//13
	{180,29},	//14
	{209,31},	//15
	{240,32},	//16
	{272,34},	//17
	{306,38},	//18
	{344,36},	//19
	{380,37},	//20
	{417,39},	//21
	{456,36},	//22
	{492,35},	//23
	{527,30},	//24
	{557,26},	//25
	{583,22},	//26
	{605,12},	//27
	{617,3},	//28
	{620,3},	//29
};

AniPoint g_AniFrameTable[] = 
{
	// 0
	{5,55,20,20,0,0},			//0

	//1
	{5, 55,10,10,0,40},			//1
	{15,55,30,20,20,0},			//2

	//2
	{5,55,10,10,0,40},			//3
	{15,55,10,10,10,40},		//4
	{25,55,10,10,20,40},		//5
	{35,55,20,20,50,0},			//6		

	//3
	{5,55,10,10,0,40},			//7
	{15,55,10,10,10,40},		//8
	{25,55,10,10,20,40},		//9
	{35,55,10,10,30,40},		//10
	{45,55,10,10,40,40},		//11
	{55,55,20,20,70,0},			//12

	// 4
	{5,55,10,10,0,40},			//13
	{15,55,10,10,10,40},		//14
	{25,55,10,10,20,40},		//15
	{35,55,10,10,30,40},		//16
	{45,55,10,10,40,40},		//17
	{55,55,10,10,50,40},		//18
	{65,55,10,10,60,40},		//19
	{75,55,20,20,90,0},			//20

	// 5
	{5,55,10,10,0,40},			//21
	{15,55,10,10,10,40},		//22
	{25,55,10,10,20,40},		//23
	{35,55,10,10,30,40},		//24
	{45,55,10,10,40,40},		//25
	{55,55,10,10,50,40},		//26
	{65,55,10,10,60,40},		//27
	{75,55,10,10,70,40},		//28
	{85,55,10,10,80,40},		//29
	{95,55,20,20,110,0},		//30

	// 6
	{5,55,10,10,0,40},			//31
	{15,55,10,10,10,40},		//32
	{25,55,10,10,20,40},		//33
	{35,55,10,10,30,40},		//34
	{45,55,10,10,40,40},		//35
	{55,55,10,10,50,40},		//36
	{65,55,10,10,60,40},		//37
	{75,55,10,10,70,40},		//38
	{85,55,10,10,80,40},		//39
	{95,55,10,10,90,40},		//40
	{105,45,30,20,130,0},		//41
	{115,65,10,10,160,0},		//42

	// 7
	{5,55,10,10,0,40},			//43
	{15,55,10,10,10,40},		//44
	{25,55,10,10,20,40},		//45
	{35,55,10,10,30,40},		//46
	{45,55,10,10,40,40},		//47
	{55,55,10,10,50,40},		//48
	{65,55,10,10,60,40},		//49
	{75,55,10,10,70,40},		//50
	{85,55,10,10,80,40},		//51
	{95,55,10,10,90,40},		//52
	{105,55,10,10,100,40},		//53
	{115,55,10,10,110,40},		//54
	{125,45,20,30,0,50},		//55

	// 8
	{5,55,10,10,0,40},			//56
	{15,55,10,10,10,40},		//57
	{25,55,10,10,20,40},		//58
	{35,55,10,10,30,40},		//59
	{45,55,10,10,40,40},		//60
	{55,55,10,10,50,40},		//61
	{65,55,10,10,60,40},		//62
	{75,55,10,10,70,40},		//63
	{85,55,10,10,80,40},		//64
	{95,55,10,10,90,40},		//65
	{105,55,10,10,100,40},		//66
	{115,55,10,10,110,40},		//67
	{125,55,10,10,120,40},		//68
	{125,55,10,10,120,40},		//69		{125,65,10,10,120,40},
	{135,55,10,10,130,40},		//70
	{135,65,20,20,0,20},		//71

	// 9
	{45,55,10,10,40,40},		//72
	{55,55,10,10,50,40},		//73
	{65,55,10,10,60,40},		//74
	{75,55,10,10,70,40},		//75
	{85,55,10,10,80,40},		//76
	{95,55,10,10,90,40},		//77
	{105,55,10,10,100,40},		//78
	{115,55,10,10,110,40},		//79
	{125,55,10,10,120,40},		//80
	{125,65,10,10,120,50},		//81
	{135,55,10,10,130,40},		//82
	{135,65,10,10,130,50},		//83
	{135,75,10,10,130,60},		//84
	{145,75,10,10,140,60},		//85
	{145,65,10,10,140,50},		//86
	{145,45,20,20,20,20},		//87
	{5,55,40,10,60,70},			//88

	// 10 
	{55,55,10,10,50,40},		//89
	{65,55,10,10,60,40},		//90
	{75,55,10,10,70,40},		//91
	{85,55,10,10,80,40},		//92
	{95,55,10,10,90,40},		//93
	{105,55,10,10,100,40},		//94
	{115,55,10,10,110,40},		//95
	{125,55,10,10,120,40},		//96
	{125,65,10,10,120,50},		//97
	{135,55,10,10,130,40},		//98
	{135,65,10,10,130,50},		//99
	{135,75,10,10,130,60},		//100
	{145,75,10,10,140,60},		//101
	{145,65,10,10,140,50},		//102
	{145,55,10,10,140,40},		//103
	{155,55,10,10,150,40},		//104
	{155,45,10,10,150,30},		//105
	{155,55,20,20,40,20},		//106
	{15,55,40,10,120,70},		//107

	// 11  
	{65,55,10,10,60,40},		//108
	{75,55,10,10,70,40},		//109
	{85,55,10,10,80,40},		//110
	{95,55,10,10,90,40},		//111
	{105,55,10,10,100,40},		//112
	{115,55,10,10,110,40},		//113
	{125,55,10,10,120,40},		//114
	{125,65,10,10,120,50},		//115
	{135,55,10,10,130,40},		//116
	{135,65,10,10,130,50},		//117
	{135,75,10,10,130,60},		//118
	{145,75,10,10,140,60},		//119
	{145,65,10,10,140,50},		//120
	{145,55,10,10,140,40},		//121
	{155,55,10,10,150,40},		//122
	{155,45,10,10,150,30},		//123
	{165,45,10,10,160,30},		//124
	{165,55,10,10,160,40},		//125
	{165,65,10,10,160,50},		//126
	{165,75,10,10,160,60},		//127
	{165,75,20,20,60,20},		//128
	{25,55,40,10,0,100},		//129

	// 12 
	{85,55,10,10,80,40},		//130
	{95,55,10,10,90,40},		//131
	{105,55,10,10,100,40},		//132
	{115,55,10,10,110,40},		//133
	{125,55,10,10,120,40},		//134
	{125,65,10,10,120,50},		//135
	{135,55,10,10,130,40},		//136
	{135,65,10,10,130,50},		//137
	{135,75,10,10,130,60},		//138
	{145,75,10,10,140,60},		//139
	{145,65,10,10,140,50},		//140
	{145,55,10,10,140,40},		//141
	{155,55,10,10,150,40},		//142
	{155,45,10,10,150,30},		//143
	{165,45,10,10,160,30},		//144
	{165,55,10,10,160,40},		//145
	{165,65,10,10,160,50},		//146
	{165,75,10,10,160,60},		//147
	{165,85,10,10,160,70},		//148
	{175,95,10,10,170,80},		//149
	{175,85,10,10,170,70},		//150
	{175,75,10,10,170,60},		//151
	{165,55,30,20,80,20},		//152
	{55,55,30,10,110,80},		//153

	// 13 
	{95, 55,10,10,90, 40},		//154
	{105,55,10,10,100,40},		//155
	{115,55,10,10,110,40},		//156
	{125,55,10,10,120,40},		//157
	{125,65,10,10,120,50},		//158
	{135,55,10,10,130,40},		//159
	{135,65,10,10,130,50},		//160
	{135,75,10,10,130,60},		//161
	{145,75,10,10,140,60},		//162
	{145,65,10,10,140,50},		//163
	{145,55,10,10,140,40},		//164
	{155,55,10,10,150,40},		//165
	{155,45,10,10,150,30},		//166
	{165,45,10,10,160,30},		//167
	{165,55,10,10,160,40},		//168
	{165,65,10,10,160,50},		//169
	{165,75,10,10,160,60},		//170
	{165,85,10,10,160,70},		//171		
	{175,95,10,10,170,80},		//172
	{175,85,10,10,170,70},		//173
	{175,75,10,10,170,60},		//174
	{175,65,10,10,170,50},		//175
	{175,55,10,10,170,40},		//176
	{175,45,10,10,170,30},		//177
	{175,25,20,20,110,20},		//178
	{65, 55,30,20,110,90},		//179

	// 14 
	{125,55,10,10,120,40},		//180
	{125,65,10,10,120,50},		//181
	{135,55,10,10,130,40},		//182
	{135,65,10,10,130,50},		//183
	{135,75,10,10,130,60},		//184
	{145,75,10,10,140,60},		//185
	{145,65,10,10,140,50},		//186
	{145,55,10,10,140,40},		//187
	{155,55,10,10,150,40},		//188
	{155,45,10,10,150,30},		//189
	{165,45,10,10,160,30},		//190
	{165,55,10,10,160,40},		//191
	{165,65,10,10,160,50},		//192
	{165,75,10,10,160,60},		//193
	{165,85,10,10,160,70},		//194	
	{175,95,10,10,170,80},		//195
	{175,85,10,10,170,70},		//196
	{175,75,10,10,170,60},		//197
	{175,65,10,10,170,50},		//198
	{175,55,10,10,170,40},		//199
	{175,45,10,10,170,30},		//200
	{175,35,10,10,170,20},		//201
	{175,25,10,10,170,10},		//202
	{175,15,10,10,170,0},		//203
	{185,15,10,10,180,0},		//204
	{185,25,10,10,180,10},		//205
	{185,35,10,10,180,20},		//206
	{185,45,20,20,130,20},		//207
	{85,55,40,10,40,100},		//208

	// 15 
	{135,65,10,10,130,50},		//209
	{135,75,10,10,130,60},		//210
	{145,75,10,10,140,60},		//211
	{145,65,10,10,140,50},		//212
	{145,55,10,10,140,40},		//213
	{155,55,10,10,150,40},		//214
	{155,45,10,10,150,30},		//215
	{165,45,10,10,160,30},		//216
	{165,55,10,10,160,40},		//217
	{165,65,10,10,160,50},		//218
	{165,75,10,10,160,60},		//219
	{165,85,10,10,160,70},		//220	
	{175,95,10,10,170,80},		//221
	{175,85,10,10,170,70},		//222
	{175,75,10,10,170,60},		//223
	{175,65,10,10,170,50},		//224
	{175,55,10,10,170,40},		//225
	{175,45,10,10,170,30},		//226
	{175,35,10,10,170,20},		//227
	{175,25,10,10,170,10},		//228
	{175,15,10,10,170,0},		//229
	{185,15,10,10,180,0},		//230
	{185,25,10,10,180,10},		//231
	{185,35,10,10,180,20},		//232
	{185,45,10,10,180,30},		//233
	{185,55,10,10,180,40},		//234
	{185,65,10,10,180,50},		//235
	{185,75,10,10,180,60},		//236
	{185,85,20,20,20,50},		//237
	{95,55,50,10,80,100},		//238
	{125,65,20,10,150,20},		//239

	// 16
	{145,55,10,10,140,40},		//240
	{155,55,10,10,150,40},		//241
	{155,45,10,10,150,30},		//242
	{165,45,10,10,160,30},		//243
	{165,55,10,10,160,40},		//244
	{165,65,10,10,160,50},		//245
	{165,75,10,10,160,60},		//246
	{165,85,10,10,160,70},		//247	
	{175,95,10,10,170,80},		//248
	{175,85,10,10,170,70},		//249
	{175,75,10,10,170,60},		//250
	{175,65,10,10,170,50},		//251
	{175,55,10,10,170,40},		//252
	{175,45,10,10,170,30},		//253
	{175,35,10,10,170,20},		//254
	{175,25,10,10,170,10},		//255
	{175,15,10,10,170,0},		//256
	{185,15,10,10,180,0},		//257
	{185,25,10,10,180,10},		//258
	{185,35,10,10,180,20},		//259
	{185,45,10,10,180,30},		//260
	{185,55,10,10,180,40},		//261
	{185,65,10,10,180,50},		//262
	{185,75,10,10,180,60},		//263
	{185,85,10,10,180,70},		//264
	{185,95,10,10,180,80},		//265
	{185,105,10,10,180,90},		//266
	{185,115,10,10,180,100},	//267
	{195,115,10,10,190,100},	//268
	{195,85,20,30,45,50},		//269		// 这里哪个点有问题
	{105,55,40,10,190,0},		//270
	{135,65,20,20,220,20},		//271

	// 17
	{165,45,10,10,160,30},		//272
	{165,55,10,10,160,40},		//273
	{165,65,10,10,160,50},		//274
	{165,75,10,10,160,60},		//275
	{165,85,10,10,160,70},		//276	
	{175,95,10,10,170,80},		//277
	{175,85,10,10,170,70},		//278
	{175,75,10,10,170,60},		//279
	{175,65,10,10,170,50},		//280
	{175,55,10,10,170,40},		//281
	{175,45,10,10,170,30},		//282
	{175,35,10,10,170,20},		//283
	{175,25,10,10,170,10},		//284
	{175,15,10,10,170,0},		//285
	{185,15,10,10,180,0},		//286
	{185,25,10,10,180,10},		//287
	{185,35,10,10,180,20},		//288
	{185,45,10,10,180,30},		//289
	{185,55,10,10,180,40},		//290
	{185,65,10,10,180,50},		//291
	{185,75,10,10,180,60},		//292
	{185,85,10,10,180,70},		//293
	{185,95,10,10,180,80},		//294
	{185,105,10,10,180,90},		//295
	{185,115,10,10,180,100},	//296
	{195,115,10,10,190,100},	//297
	{195,105,10,10,190,90},		//298
	{195,95,10,10,190,80},		//299
	{195,85,10,10,190,70},		//300
	{195,75,10,10,190,60},		//301
	{195,55,20,20,60,50},		//302
	{155,45,10,20,150,50},		//303
	{135,55,20,30,240,0},		//304
	{125,55,10,10,160,10},		//305

	// 18
	{165,65,10,10,160,50},		//306
	{165,75,10,10,160,60},		//307
	{165,85,10,10,160,70},		//308
	{175,95,10,10,170,80},		//309
	{175,85,10,10,170,70},		//310
	{175,75,10,10,170,60},		//311
	{175,65,10,10,170,50},		//312
	{175,55,10,10,170,40},		//313
	{175,45,10,10,170,30},		//314
	{175,35,10,10,170,20},		//315
	{175,25,10,10,170,10},		//316
	{175,15,10,10,170,0},		//317
	{185,15,10,10,180,0},		//318
	{185,25,10,10,180,10},		//319
	{185,35,10,10,180,20},		//320
	{185,45,10,10,180,30},		//321
	{185,55,10,10,180,40},		//322
	{185,65,10,10,180,50},		//323
	{185,75,10,10,180,60},		//324
	{185,85,10,10,180,70},		//325
	{185,95,10,10,180,80},		//326
	{185,105,10,10,180,90},		//327
	{185,115,10,10,180,100},	//328
	{195,115,10,10,190,100},	//329
	{195,105,10,10,190,90},		//330
	{195,95,10,10,190,80},		//331
	{195,85,10,10,190,70},		//332
	{195,75,10,10,190,60},		//333
	{195,65,10,10,190,50},		//334
	{205,75,10,10,200,60},		//335
	{205,65,10,10,200,50},		//336
	{205,55,10,10,200,40},		//337
	{205,45,10,10,200,30},		//338
	{205,35,10,10,200,20},		//339
	{205,25,10,10,200,10},		//340
	{205,35,20,20,80,50},		//341
	{155,45,20,20,260,0},		//342
	{145,55,10,30,190,10},		//343

	// 19
	{175,85,10,10,170,70},		//344
	{175,75,10,10,170,60},		//345
	{175,65,10,10,170,50},		//346
	{175,55,10,10,170,40},		//347
	{175,45,10,10,170,30},		//348
	{175,35,10,10,170,20},		//349
	{175,25,10,10,170,10},		//350
	{175,15,10,10,170,0},		//351
	{185,15,10,10,180,0},		//352
	{185,25,10,10,180,10},		//353
	{185,35,10,10,180,20},		//354
	{185,45,10,10,180,30},		//355
	{185,55,10,10,180,40},		//356
	{185,65,10,10,180,50},		//357
	{185,75,10,10,180,60},		//358
	{185,85,10,10,180,70},		//359
	{185,95,10,10,180,80},		//360
	{185,105,10,10,180,90},		//361
	{185,115,10,10,180,100},	//362		
	{195,115,10,10,190,100},	//363
	{195,105,10,10,190,90},		//364
	{195,95,10,10,190,80},		//365
	{195,85,10,10,190,70},		//366
	{195,75,10,10,190,60},		//367
	{195,65,10,10,190,50},		//368
	{205,75,10,10,200,60},		//369
	{205,65,10,10,200,50},		//370
	{205,55,10,10,200,40},		//371
	{205,45,10,10,200,30},		//372
	{205,35,10,10,200,20},		//373
	{205,25,10,10,200,10},		//374
	{215,35,10,10,210,20},		//375
	{215,45,10,10,210,30},		//376
	{215,55,20,30,100,50},		//377
	{165,55,10,40,200,70},		//378
	{155,45,10,20,230,0},		//379

	// 20
	{175,45,10,10,170,30},		//380
	{175,35,10,10,170,20},		//381
	{175,25,10,10,170,10},		//382
	{175,15,10,10,170,0},		//383
	{185,15,10,10,180,0},		//384
	{185,25,10,10,180,10},		//385
	{185,35,10,10,180,20},		//386
	{185,45,10,10,180,30},		//387
	{185,55,10,10,180,40},		//388
	{185,65,10,10,180,50},		//389
	{185,75,10,10,180,60},		//390
	{185,85,10,10,180,70},		//391
	{185,95,10,10,180,80},		//392
	{185,105,10,10,180,90},		//393
	{185,115,10,10,180,100},	//394
	{195,115,10,10,190,100},	//395
	{195,105,10,10,190,90},		//396
	{195,95,10,10,190,80},		//397
	{195,85,10,10,190,70},		//398
	{195,75,10,10,190,60},		//399
	{195,65,10,10,190,50},		//400
	{205,75,10,10,200,60},		//401
	{205,65,10,10,200,50},		//402
	{205,55,10,10,200,40},		//403
	{205,45,10,10,200,30},		//404
	{205,35,10,10,200,20},		//405
	{205,25,10,10,200,10},		//406
	{215,35,10,10,210,20},		//407
	{215,45,10,10,210,30},		//408
	{215,55,10,10,210,40},		//409
	{215,65,10,10,210,50},		//410
	{225,55,10,10,220,40},		//411
	{225,65,10,10,220,50},		//412
	{225,75,10,10,220,60},		//413
	{235,75,10,10,230,60},		//414
	{235,45,20,30,20,70},		//415
	{165,55,20,40,280,0},		//416

	// 21
	{175,15,10,10,170,0},		//417
	{185,15,10,10,180,0},		//418
	{185,25,10,10,180,10},		//419
	{185,35,10,10,180,20},		//420
	{185,45,10,10,180,30},		//421
	{185,55,10,10,180,40},		//422
	{185,65,10,10,180,50},		//423
	{185,75,10,10,180,60},		//424
	{185,85,10,10,180,70},		//425
	{185,95,10,10,180,80},		//426
	{185,105,10,10,180,90},		//427
	{185,115,10,10,180,100},	//428
	{195,115,10,10,190,100},	//429
	{195,105,10,10,190,90},		//430
	{195,95,10,10,190,80},		//431
	{195,85,10,10,190,70},		//432
	{195,75,10,10,190,60},		//433
	{195,65,10,10,190,50},		//434
	{205,75,10,10,200,60},		//435
	{205,65,10,10,200,50},		//436
	{205,55,10,10,200,40},		//437
	{205,45,10,10,200,30},		//438
	{205,35,10,10,200,20},		//439
	{205,25,10,10,200,10},		//440
	{215,35,10,10,210,20},		//441
	{215,45,10,10,210,30},		//442
	{215,55,10,10,210,40},		//443
	{215,65,10,10,210,50},		//444
	{225,55,10,10,220,40},		//445
	{225,65,10,10,220,50},		//446
	{225,75,10,10,220,60},		//447
	{235,75,10,10,230,60},		//448
	{235,65,10,10,230,50},		//449
	{235,55,10,10,230,40},		//450
	{245,55,10,10,240,40},		//451
	{245,45,10,10,240,30},		//452
	{255,55,20,20,0,80},		//453
	{175,25,10,50,210,60},		//454
	{165,75,20,20,260,20},		//455

	// 22
	{185,55,10,10,180,40},		//456
	{185,65,10,10,180,50},		//457
	{185,75,10,10,180,60},		//458
	{185,85,10,10,180,70},		//459
	{185,95,10,10,180,80},		//460
	{185,105,10,10,180,90},		//461
	{185,115,10,10,180,100},	//462
	{195,115,10,10,190,100},	//463
	{195,105,10,10,190,90},		//464
	{195,95,10,10,190,80},		//465
	{195,85,10,10,190,70},		//466
	{195,75,10,10,190,60},		//467
	{195,65,10,10,190,50},		//468
	{205,75,10,10,200,60},		//469
	{205,65,10,10,200,50},		//470
	{205,55,10,10,200,40},		//471
	{205,45,10,10,200,30},		//472
	{205,35,10,10,200,20},		//473
	{205,25,10,10,200,10},		//474
	{215,35,10,10,210,20},		//475
	{215,45,10,10,210,30},		//476
	{215,55,10,10,210,40},		//477
	{215,65,10,10,210,50},		//478
	{225,55,10,10,220,40},		//479
	{225,65,10,10,220,50},		//480
	{225,75,10,10,220,60},		//481
	{235,75,10,10,230,60},		//482
	{235,65,10,10,230,50},		//483
	{235,55,10,10,230,40},		//484
	{245,55,10,10,240,40},		//485
	{245,45,10,10,240,30},		//486
	{255,55,10,10,250,40},		//487
	{265,55,10,10,260,40},		//488
	{275,55,20,20,40,80},		//489
	{175,15,20,40,300,0},		//490
	{175,55,10,40,320,0},		//491

	// 23
	{185,95,10,10,180,80},		//492
	{185,105,10,10,180,90},		//493
	{185,115,10,10,180,100},	//494
	{195,115,10,10,190,100},	//495
	{195,105,10,10,190,90},		//496
	{195,95,10,10,190,80},		//497
	{195,85,10,10,190,70},		//498
	{195,75,10,10,190,60},		//499
	{195,65,10,10,190,50},		//500
	{205,75,10,10,200,60},		//501
	{205,65,10,10,200,50},		//502
	{205,55,10,10,200,40},		//503
	{205,45,10,10,200,30},		//504
	{205,35,10,10,200,20},		//505
	{205,25,10,10,200,10},		//506
	{215,35,10,10,210,20},		//507
	{215,45,10,10,210,30},		//508
	{215,55,10,10,210,40},		//509
	{215,65,10,10,210,50},		//510
	{225,55,10,10,220,40},		//511
	{225,65,10,10,220,50},		//512
	{225,75,10,10,220,60},		//513
	{235,75,10,10,230,60},		//514
	{235,65,10,10,230,50},		//515
	{235,55,10,10,230,40},		//516
	{245,55,10,10,240,40},		//517
	{245,45,10,10,240,30},		//518
	{255,55,10,10,250,40},		//519
	{265,55,10,10,260,40},		//520
	{275,55,10,10,270,40},		//521
	{285,55,30,20,140,80},		//522
	{175,15,20,20,330,20},		//523
	{175,35,10,10,250,30},		//524
	{185,35,10,60,240,50},		//525
	{185,95,20,30,250,50},		//526

	// 24
	{195,85,10,10,190,70},		//527
	{195,75,10,10,190,60},		//528
	{195,65,10,10,190,50},		//529
	{205,75,10,10,200,60},		//530
	{205,65,10,10,200,50},		//531
	{205,55,10,10,200,40},		//532
	{205,45,10,10,200,30},		//533
	{205,35,10,10,200,20},		//534
	{205,25,10,10,200,10},		//535
	{215,35,10,10,210,20},		//536
	{215,45,10,10,210,30},		//537
	{215,55,10,10,210,40},		//538
	{215,65,10,10,210,50},		//539
	{225,55,10,10,220,40},		//540
	{225,65,10,10,220,50},		//541
	{225,75,10,10,220,60},		//542
	{235,75,10,10,230,60},		//543
	{235,65,10,10,230,50},		//544
	{235,55,10,10,230,40},		//545
	{245,55,10,10,240,40},		//546
	{245,45,10,10,240,30},		//547
	{255,55,10,10,250,40},		//548
	{265,55,10,10,260,40},		//549
	{275,55,10,10,270,40},		//550
	{285,55,10,10,280,40},		//551
	{295,55,10,10,290,40},		//552
	{305,55,30,20,60,80},		//553
	{185,15,10,40,220,70},		//554
	{185,55,10,40,230,70},		//555
	{185,95,20,30,250,80},		//556

	// 25
	{205,45,10,10,200,30},		//557
	{205,35,10,10,200,20},		//558
	{205,25,10,10,200,10},		//559
	{215,35,10,10,210,20},		//560
	{215,45,10,10,210,30},		//561
	{215,55,10,10,210,40},		//562
	{215,65,10,10,210,50},		//563
	{225,55,10,10,220,40},		//564
	{225,65,10,10,220,50},		//565
	{225,75,10,10,220,60},		//566
	{235,75,10,10,230,60},		//567
	{235,65,10,10,230,50},		//568
	{235,55,10,10,230,40},		//569
	{245,55,10,10,240,40},		//570
	{245,45,10,10,240,30},		//571
	{255,55,10,10,250,40},		//572
	{265,55,10,10,260,40},		//573
	{275,55,10,10,270,40},		//574
	{285,55,10,10,280,40},		//575
	{295,55,10,10,290,40},		//576
	{305,55,10,10,300,40},		//577
	{315,55,10,10,310,40},		//578
	{325,55,20,20,90,80},		//579
	{185,95,20,30,270,50},		//580
	{195,65,20,30,330,60},		//581
	{205,55,10,10,120,60},		//582

	// 26
	{215,55,10,10,210,40},		//583
	{215,65,10,10,210,50},		//584
	{225,55,10,10,220,40},		//585
	{225,65,10,10,220,50},		//586
	{225,75,10,10,220,60},		//587
	{235,75,10,10,230,60},		//588
	{235,65,10,10,230,50},		//589
	{235,55,10,10,230,40},		//590
	{245,55,10,10,240,40},		//591
	{245,45,10,10,240,30},		//592
	{255,55,10,10,250,40},		//593
	{265,55,10,10,260,40},		//594
	{275,55,10,10,270,40},		//595
	{285,55,10,10,280,40},		//596
	{295,55,10,10,290,40},		//597
	{305,55,10,10,300,40},		//598
	{315,55,10,10,310,40},		//599
	{325,55,10,10,320,40},		//600
	{335,55,10,10,330,40},		//601
	{335,55,20,20,330,0},		//602
	{195,55,20,30,290,50},		//603
	{205,25,20,30,290,80},		//604

	// 27
	{265,55,10,10,260,40},		//605
	{275,55,10,10,270,40},		//606
	{285,55,10,10,280,40},		//607
	{295,55,10,10,290,40},		//608
	{305,55,10,10,300,40},		//609
	{315,55,10,10,310,40},		//610
	{325,55,10,10,320,40},		//611
	{335,55,10,10,330,40},		//612
	{345,55,10,10,340,40},		//613
	{215,35,10,40,310,50},		//614
	{225,65,20,20,330,90},		//615
	{235,55,30,10,320,50},		//616

	// 28
	{265,55,50,10,130,100},		//617
	{315,55,20,10,270,80},		//618
	{335,55,18,10,270,90},		//619

	// 29
	{305,55,20,10,270,100},		//620
	{325,55,20,10,310,90},		//621
	{345,55,8, 10,170,90},		//622

};

UINT PaintAniThread(LPVOID lp)
{
	CAutoUpdateClientDlg* pMainDlg = (CAutoUpdateClientDlg*)lp;

	if(!pMainDlg) return 0;

	RECT rect;
	rect.left = 8;
	rect.top = 8;
	rect.right = 348;
	rect.bottom = 120;

	while(TRUE)
	{
		InvalidateRect(pMainDlg->m_hWnd, &rect, TRUE);
		pMainDlg->AniTick();
		
		Sleep(50);
	}
}

CAutoUpdateClientDlg::CAutoUpdateClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoUpdateClientDlg::IDD, pParent)
	, m_dwCount(0)
	, m_nMaxDownThread(5)
	, m_nCurThreadCount(0)
	//, m_bTray(false)
	, m_InstanceChecker(_T("DroiyanUpdate"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_APPMAIN);
	m_bNeedExecute = FALSE;
	m_ExecuteFile = _T("啄木鸟决战登陆器.exe");   //必须与登录器名字一致
	m_ExecuteParam = _T("-SANLE_JUEZHAN");
	m_UpdateUrl = _T("");

	m_bMouceDown = FALSE;

	m_AniFrame = 3;
}

void CAutoUpdateClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_CANCELBUTTON, m_CancelButton);
	DDX_Control(pDX, ID_SZINFO, TextInfo);
	DDX_Control(pDX, IDC_SZINFOCONTENT, TextInfoContent);
	DDX_Control(pDX, IDC_NEWPROGRESS, m_progDownLoad);
}

BEGIN_MESSAGE_MAP(CAutoUpdateClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DOWNLOADFILE, OnDownLoadFile) 
	ON_MESSAGE(WM_DOWNLOADFILEEND, OnDownLoadFileEnd) 
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(ID_CANCELBUTTON, &CAutoUpdateClientDlg::OnBnClickedCancelbutton)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_DRAWITEM()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CAutoUpdateClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//CFont* font;
	//font=this->GetFont();//获取CFont对象
	//LOGFONT lf;
	//font->GetLogFont(&lf);//获取LOGFONT结构体
	//lf.lfHeight=-26;    //修改字体大小
	//lf.lfItalic=TRUE;        //倾斜
	//lf.lfWeight=400;   //修改字体的粗细
	//newFont.CreateFontIndirectW(&lf);//创建一个新的字体
	//m_static_xsdj.SetFont(&newFont);

	LOGFONT logFont;
	m_Font.CreatePointFont(12,_T("宋体"), NULL);
	m_Font.GetLogFont(&logFont);
	logFont.lfCharSet = GB2312_CHARSET;
	logFont.lfHeight = 12;
	//logFont.lfWidth = 6;
	logFont.lfWeight = FW_BOLD;
	m_Font.CreateFontIndirect(&logFont);

	//SetFont(&m_Font);
	
	//TextInfo.SetFont(&m_Font);
	//TextInfoContent.SetFont(&m_Font);

	m_AniBitmap.LoadBitmap(IDB_BITMAP5);

	//SetWindowPos(&wndTopMost , 0 , 0 , 350 , 92 , SWP_NOMOVE);
	CRect rect;
	GetClientRect(&rect);
	MoveWindow(rect.top , rect.left , 358 , 190 , TRUE);

	//移动CANCEL
	m_CancelButton.SetWindowPos(NULL, 220, 155, 132, 30, SWP_SHOWWINDOW);
	m_progDownLoad.SetWindowPos(NULL, 8, 159, 203, 22, SWP_SHOWWINDOW);

	TextInfo.SetWindowPos(NULL, 10, 118, 400, 18, SWP_SHOWWINDOW);
	TextInfo.SetWindowText(_T("正在连接决战游戏服务器."));

	TextInfoContent.SetWindowPos(NULL , 10, 136, 400, 18, SWP_SHOWWINDOW);
	TextInfoContent.SetWindowText(_T(""));

	SetTimer(PUBLIC_TIMER, 1000, NULL);
	//SetTimer(UM_ANIMATION, 50, NULL);

	AfxBeginThread(PaintAniThread, this);


	/*if ( ! m_bTray )
	{
		m_pTray.cbSize				= sizeof(m_pTray);
		m_pTray.hWnd				= GetSafeHwnd();
		m_pTray.uID					= 0;
		m_pTray.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_pTray.uCallbackMessage	= WM_TRAY;
		m_pTray.hIcon				= theApp.LoadIcon( IDI_ICON1);
		_tcscpy( m_pTray.szTip, _T("正在自动更新") );
		Shell_NotifyIcon( NIM_ADD, &m_pTray );
		ShowWindow( SW_HIDE );
		m_bTray = true;
	}*/

	m_InstanceChecker.TrackFirstInstanceRunning();

#ifdef TEST_DOWNLOAD
	SetTimer(UM_TESDOWN , 50 , NULL);
#else
	StartUpload();		//开始进行自动更新
#endif
	return true; 
}

void CAutoUpdateClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//Invalidate(TRUE);
		CDialog::OnPaint();
		//return;
		int y = 0;
		int index = 0, pointsize = 0;
		CDC* dc = GetDC();

		CDC bitmapDC ;
		if(bitmapDC.CreateCompatibleDC(dc) == TRUE)
		{
			bitmapDC.SelectObject(&m_AniBitmap);

			index = g_AniFrameData[m_AniFrame].start;
			pointsize = g_AniFrameData[m_AniFrame].size;
			for(int i = 0; i < pointsize; i++)
			{
				dc->BitBlt(
					g_AniFrameTable[index].x, 
					g_AniFrameTable[index].y, 
					g_AniFrameTable[index].cx, 
					g_AniFrameTable[index].cy, 
					&bitmapDC, 
					g_AniFrameTable[index].fx, 
					g_AniFrameTable[index].fy, 
					SRCCOPY); 
				index++;
			}

			bitmapDC.DeleteDC();
			
		}

		//CDialog::OnPaint();
		return;
	}
}

HCURSOR CAutoUpdateClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoUpdateClientDlg::SetUpdateUrl(CString strUrl)
{
	m_UpdateUrl = strUrl;
}

void CAutoUpdateClientDlg::StartUpload()
{
	TCHAR UrlPath[250];
	::ZeroMemory(UrlPath , 250);

	_stprintf(UrlPath, _T("http://%s/%s"), m_UpdateUrl, UPDATE_PATH);

	
	CServer pServer ;
	pServer.m_sServerName = _T("啄木鸟决战更新程序");//自动更新显示
	pServer.m_sUpdateFileXmlURL = UrlPath;


	USES_CONVERSION;
	TRACE("%s\n", T2A(pServer.m_sUpdateFileXmlURL));

	TCHAR szTempPath[MAX_PATH], szTempName[MAX_PATH];
	::GetTempPath(
		MAX_PATH,
		szTempPath);

	GetTempFileName(szTempPath, __TEXT("Xml"), 0, szTempName);

	m_RemoteFiles.ClearAllList();

	CString szTemp;
	szTemp.Format(_T("正在连接：%s."), pServer.m_sServerName);
	AddLog(szTemp);
	CInetFile *pInetFileNew = new CInetFile();
	if (pInetFileNew) 
	{
		pInetFileNew->m_bProgressMode = true;
		pInetFileNew->m_dlFileType = FILELIST_XML;
		pInetFileNew->m_hWnd = m_hWnd;
		pInetFileNew->m_sURL = pServer.m_sUpdateFileXmlURL;
		pInetFileNew->m_sServerName = pServer.m_sServerName;
		pInetFileNew->m_sPath = szTempName;
		pInetFileNew->m_dwTotalLen = 0;
		pInetFileNew->m_sFileName = _T("");
		m_InetFiles.Push(pInetFileNew);
	}
}

LRESULT CAutoUpdateClientDlg::OnDownLoadFile(WPARAM wParam, LPARAM lParam) 
{ 
	CKevinAutoLock  lock(&m_lock);

	int nLen = LOWORD(wParam);
	DOWNLOAD_FILE_TYPE llFileType = (DOWNLOAD_FILE_TYPE)HIWORD(wParam);
	CInetFile *pInetFile = (CInetFile *)lParam;
	CString szTemp;
	ASSERT(pInetFile);

	TRACE("wParam = %u, lParam = %u\n", wParam, lParam);
	if (pInetFile) 
	{
		TCHAR szText[64];		
		_tcscpy(szText, _T(""));
		if (pInetFile->m_dwTotalLen > 0) 
		{
			int nPercent = pInetFile->m_nAlreadyDown * 100 / pInetFile->m_dwTotalLen;
			szTemp.Format(_T("加载速度：%dKB/%dKB."), pInetFile->m_nAlreadyDown, pInetFile->m_dwTotalLen);
			AddLogContent(szTemp);

			m_progDownLoad.SetPos(nPercent);
			if(nPercent >= 100) m_progDownLoad.Invalidate(TRUE);
			//DWORD dwCurWidth = nPercent * 100 / m_DownloadProgress.GetMaxWidth() ;
			//m_DownloadProgress.SetWidth(dwCurWidth);
			//m_DownloadProgress.MoveWindow( 0 , 0 ,  ,rect.Height() ,TRUE);
		}

		//托盘ICON
		/*if ( m_bTray )
		{			
			static int nCount = 0;
			nCount ++;
			switch (nCount % 3)
			{
			case 0:
				m_pTray.hIcon = theApp.LoadIcon( IDI_ICON1);
				break;
			case 1:
				m_pTray.hIcon = theApp.LoadIcon( IDI_ICON2);
				break;
			case 2:
				m_pTray.hIcon = theApp.LoadIcon( IDI_ICON3);
				break;
			}

			m_pTray.uFlags = NIF_ICON;
			Shell_NotifyIcon( NIM_MODIFY, &m_pTray );

			if ( _tcsncmp( m_pTray.szTip, szText, 63 ) )
			{				
				m_pTray.uFlags = NIF_TIP;
				_tcsncpy( m_pTray.szTip, szText, 63 );
				Shell_NotifyIcon( NIM_MODIFY, &m_pTray );
			}
		}*/
	}

	return 0; 
} 

bool FileExist(CString sFileName)
{
	HANDLE hFile = CreateFile(sFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	::CloseHandle(hFile);

	return true;
}

LRESULT CAutoUpdateClientDlg::OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam) 
{ 
	// wParam 0 成功, 2, SESSION 异常 3, 主动停止
	CKevinAutoLock  lock(&m_lock);
	CString sLog; 

	int nRetCode = LOWORD(wParam);
	DOWNLOAD_FILE_TYPE llFileType = (DOWNLOAD_FILE_TYPE)HIWORD(wParam);
	CInetFile *pInetFile = (CInetFile *)lParam;
	ASSERT(pInetFile);

	switch(llFileType)
	{
	case FILELIST_XML:		// 下载完成 XML FILE LIST文件
		{
			if (0 == nRetCode) 
			{
				sLog.Format(_T("获取更新列表成功：%s"), pInetFile->m_sServerName);
				AddLog(sLog);

				if (m_RemoteFiles.ReadFromXMLFile((LPTSTR)(LPCTSTR)pInetFile->m_sPath)) 
				{
					m_dwCount = m_RemoteFiles.CheckNeedUpdate();
					if (0 == m_dwCount) 
					{
						sLog.Format(_T("当前登录器版本是最新的,正在启动游戏."));
						AddLogContent(sLog, nRetCode);
						RunGame();
					}
					else 
					{
						m_dwUpdatedCount = 0;
						m_dwUpdateTotalBytes = m_RemoteFiles.GetNeedUpdateTotalLength();
						sLog.Format(_T("%d/更新%d个文件."), m_dwUpdatedCount, m_dwCount);
						AddLog(sLog);
					}

					CUpdateFile *pUpdateFile = m_RemoteFiles.PopUpdateFile();
					while(pUpdateFile) 
					{
						CInetFile *pInetFileNew = new CInetFile();
						if (pInetFileNew) 
						{
							pInetFileNew->m_bProgressMode = true;
							pInetFileNew->m_dlFileType = GENERAL_FILE;
							pInetFileNew->m_hWnd = m_hWnd;
							pInetFileNew->m_sURL = pUpdateFile->m_sURL;

							if(pInetFileNew->m_sURL.Left(4).CompareNoCase(_T("http")) != 0)
							{
								CString szTemp = _T("");
								szTemp.Format(_T("http://%s/%s") , m_UpdateUrl , pInetFileNew->m_sURL);
								pInetFileNew->m_sURL = szTemp ;
							}

							pInetFileNew->m_sServerName = pInetFile->m_sServerName;
							pInetFileNew->m_sPath = pUpdateFile->m_sFilePath;
							//pInetFileNew->m_dwTotalLen = (DWORD)pUpdateFile->m_ullSize;
							pInetFileNew->m_sFileName = pUpdateFile->m_sFileName;

							//pInetFileNew->m_bRegister = pUpdateFile->m_bRegister;
							//pInetFileNew->m_bZipFile = pUpdateFile->m_bZipFile;
							//pInetFileNew->m_bForceOverwrite = pUpdateFile->m_bForceOverwrite;
							//pInetFileNew->m_bExecute = pUpdateFile->m_bExecute;
							//pInetFileNew->m_sExecutePar = pUpdateFile->m_sExecutePar;
							pInetFileNew->m_sMD5 =  pUpdateFile->m_sMD5;

							m_InetFiles.Push(pInetFileNew);

							SAFE_DELETE(pUpdateFile);
							pUpdateFile = m_RemoteFiles.PopUpdateFile();
						}
					}
				}

				::DeleteFile(pInetFile->m_sPath);
			}
			else
			{
				sLog.Format(_T("更新失败：%s[%s]."), pInetFile->GetLastError(), pInetFile->m_sServerName);
				AddLog(sLog, nRetCode);
				Sleep(2000);
				RunGame();
			}
		}
		break;

	case GENERAL_FILE:		// 下载完成普通更新文件
		{
			if (0 == nRetCode) 
			{	// 下载成功
				// 删除原来的文件，改名
				USES_CONVERSION;
				if (FileExist(pInetFile->m_sPath)) 
				{
					if (::DeleteFile(pInetFile->m_sPath)) 
					{
						RenameAndExeFile(pInetFile);
					}
					else 
					{
						sLog.Format(_T("更新提示：删除文件[%s]失败."), pInetFile->m_sFileName);
						AddLogContent(sLog, nRetCode);
					}
				}
				else 
				{
					RenameAndExeFile(pInetFile);
				}
			}
			else
			{
				sLog.Format(_T("更新提示：下载文件[%s]失败[%u]."), pInetFile->m_sFileName, nRetCode);
				//sLog.Format(_T("下载文件失败 [%s] ,请检查是否正在运行.") ,  pInetFile->m_sFileName);
				AddLogContent(sLog, nRetCode);
			}
		}
		break;
	}

	m_InetFiles.DeleteItem(pInetFile);
	if (m_InetFiles.Count() == 0) 
	{
		if (GENERAL_FILE == llFileType) 
		{
			sLog.Format(_T("更新完成,正在启动游戏."));
			AddLog(sLog, nRetCode);
			RunGame();
			DestroyWindow();
			//
		}
	}

	m_nCurThreadCount --;
	return 0; 
}

void CAutoUpdateClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case PUBLIC_TIMER:
		{
			CKevinAutoLock  lock(&m_lock);
			if (m_nCurThreadCount < m_nMaxDownThread) 
			{
				CInetFile* pInetFile = m_InetFiles.Front();
				if (pInetFile && 0 == pInetFile->m_nDownloadTime) 
				{
					if (GENERAL_FILE == pInetFile->m_dlFileType) 
					{
						m_progDownLoad.SetPos(0);

						CString szTemp;
						if(m_dwCount == 0)
						{
							szTemp.Format(_T("正在连接文件：%s."), pInetFile->m_sFileName);
							AddLog(szTemp);
						}
						else
						{
							m_dwUpdatedCount += 1;
							szTemp.Format(_T("%d/更新%d个文件."), m_dwUpdatedCount, m_dwCount);
							AddLog(szTemp);
						}
						//AddLogContent(sLog, 0);
					}
					if (pInetFile->GetInetFile())
						m_nCurThreadCount ++;
				}
			}
		}
		break;
	case UM_TESDOWN :
		{
			int pos = m_progDownLoad.GetPos();
			m_progDownLoad.SetPos(pos+1);

			CString szTemp;
			szTemp.Format(TEXT("更新进度 %d/%d."), m_progDownLoad.GetPos(), 100);
			AddLogContent(szTemp);
		}
		break;
	case UM_ANIMATION:
		RECT rect;
		rect.left = 8;
		rect.top = 8;
		rect.right = 348;
		rect.bottom = 120;
		Invalidate(); // Rect(&rect, TRUE);
		m_AniFrame += 1;
		if(m_AniFrame >= 30)
			m_AniFrame = 0;
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CAutoUpdateClientDlg::AddLogContent(LPCTSTR sLog, int nErrCode)
{
	TextInfoContent.SetWindowText(sLog);

	//CRect rect;
	//TextInfoContent.GetWindowRect(&rect);
	//ScreenToClient(&rect);
	//InvalidateRect(&rect);
	
	//TextInfoContent.ShowWindow(SW_HIDE);
	//TextInfoContent.ShowWindow(SW_SHOW);
	//CString sAddString;
	//CTime t = CTime::GetCurrentTime();
	//sAddString.Format(_T("%.2u:%.2u:%.2u %s"), t.GetHour(), t.GetMinute(), t.GetSecond(), sLog);
	////sAddString.Format(_T("%s Servers:%u, InetFiles:%u, RemoteFiles:%u, %.2u:%.2u:%.2u"),
	////	sLog, m_Servers.Count(), m_InetFiles.Count(), m_RemoteFiles.Count(),
	////	t.GetHour(), t.GetMinute(), t.GetSecond());
	//USES_CONVERSION;
	//TRACE("%s\n", T2A(sAddString));
	//int nItem = m_lstLog.AddString(sAddString);
	//m_lstLog.SetItemData(nItem, nErrCode);
	//m_lstLog.SetCurSel(nItem);
}

void CAutoUpdateClientDlg::AddLog(LPCTSTR sLog, int nErrCode)
{
	CString szInfo;
	TextInfo.GetWindowText(szInfo);
	if(szInfo.CompareNoCase(sLog) != 0)
	{
		TextInfo.SetWindowText(sLog);
		//TextInfo.ShowWindow(SW_HIDE);
		//TextInfo.ShowWindow(SW_SHOW);
	}
	//CString sAddString;
	//CTime t = CTime::GetCurrentTime();
	//sAddString.Format(_T("%.2u:%.2u:%.2u %s"), t.GetHour(), t.GetMinute(), t.GetSecond(), sLog);
	////sAddString.Format(_T("%s Servers:%u, InetFiles:%u, RemoteFiles:%u, %.2u:%.2u:%.2u"),
	////	sLog, m_Servers.Count(), m_InetFiles.Count(), m_RemoteFiles.Count(),
	////	t.GetHour(), t.GetMinute(), t.GetSecond());
	//USES_CONVERSION;
	//TRACE("%s\n", T2A(sAddString));
	//int nItem = m_lstLog.AddString(sAddString);
	//m_lstLog.SetItemData(nItem, nErrCode);
	//m_lstLog.SetCurSel(nItem);
}

void CAutoUpdateClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	UINT nCommand = nID & 0xFFF0;
	BOOL bShift = GetAsyncKeyState( VK_SHIFT ) & 0x8000;

	if ( nCommand == SC_MINIMIZE || ( nCommand == SC_MINIMIZE && bShift ) )
	{
		return;
	}

	CDialog::OnSysCommand(nID, lParam);
}

BOOL CAutoUpdateClientDlg::DestroyWindow()
{
	/*if ( m_bTray )
	{
		Shell_NotifyIcon( NIM_DELETE, &m_pTray );
		m_bTray = false;
	}*/

	m_AniBitmap.DeleteObject();

	return CDialog::DestroyWindow();
}

void CAutoUpdateClientDlg::RenameAndExeFile(CInetFile* pInetFile)
{
	ASSERT(pInetFile);

	CString sLog;

	CString zipFilePath = pInetFile->m_sPath + _T(".zip");
	sLog.Format(_T("正在解压：文件[%s]."), zipFilePath);
	AddLogContent(sLog, 0);

	
	USES_CONVERSION;

	if(UnZipFile((LPTSTR)(LPCTSTR)pInetFile->m_sPath, (LPTSTR)(LPCTSTR)zipFilePath))
	//if (0 == ::rename(T2A(pInetFile->m_sPath + _T(".zip")), T2A(pInetFile->m_sPath))) 
	{
		sLog.Format(_T("解压成功：文件[%s]."), pInetFile->m_sFileName);
		AddLogContent(sLog, 0);

		/*
		从下载XML中获取需要执行的文件
		if (pInetFile->m_bExecute) 
		{
			m_ExecuteFile = pInetFile->m_sPath;
			m_ExecuteParam = pInetFile->m_sExecutePar;
			m_bNeedExecute = TRUE;
		}*/

		if(pInetFile->m_sFileName.CompareNoCase(_T("LICENSE.LIB")) == 0)
		{
			TCHAR szIniPath[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, szIniPath);
			_tcscat(szIniPath, _T("\\DroiyanOnline.ini"));
			WritePrivateProfileString(_T("LICENSE"), _T("CHECK"), _T("0"), szIniPath);
		}
	}
	else 
	{
		sLog.Format(_T("解压失败：文件[%s]."), pInetFile->m_sFileName);
		AddLogContent(sLog, 0);
	}
	m_progDownLoad.SetPos(0);
}

void CAutoUpdateClientDlg::RunGame()
{
	//获取当前目录
	TCHAR strUpdateFile[MAX_PATH];
	CString strExeFile;
	CString strExeFolder;
	::GetModuleFileName(NULL,strUpdateFile,MAX_PATH);
	strExeFile.Format(_T("%s"),strUpdateFile);
	int iPos = strExeFile.ReverseFind('\\');
	strExeFolder.Format( _T("%s\\%s") , strExeFile.Left(iPos ) , m_ExecuteFile);

	::ShellExecute(NULL, NULL, strExeFolder.GetBuffer(0) , m_ExecuteParam, NULL, SW_SHOW);
	strExeFolder.ReleaseBuffer(0);
	strExeFile.ReleaseBuffer(0);
	DestroyWindow();
}


void CAutoUpdateClientDlg::OnBnClickedCancelbutton()
{
	EndDialog(IDCANCEL);
}


BOOL CAutoUpdateClientDlg::OnEraseBkgnd(CDC* pDC)
{
	CBitmap bgBitmap ;
	bgBitmap.LoadBitmap(IDB_BGMAIN);

	CDC comDC;
	comDC.CreateCompatibleDC(pDC);
	comDC.SelectObject(&bgBitmap);

	CRect rect;
	GetClientRect(&rect);

	pDC->BitBlt(rect.left , rect.top , rect.Width() , rect.Height() , &comDC , 0 , 0 , SRCCOPY);
	return TRUE;
}


HBRUSH CAutoUpdateClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == ID_SZINFO || pWnd->GetDlgCtrlID() == IDC_SZINFOCONTENT)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0x80 , 0xFF , 0x80));
		HBRUSH trans = (HBRUSH)GetStockObject(NULL_BRUSH) ;

		//CBrush *trans = CBrush::FromHandle(trans);

		return (HBRUSH) trans; 
	}

	if(pWnd->GetDlgCtrlID() == IDC_NEWPROGRESS)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH) ;
	}

	return hbr;
}

void CAutoUpdateClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouceDown = TRUE;
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y)); 
}


void CAutoUpdateClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bMouceDown == TRUE)
	{
		CRect rect;
		GetClientRect(&rect);

		ClientToScreen(&rect);
		
		MoveWindow(rect.left  , rect.top  , rect.Width() , rect.Height() , TRUE);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CAutoUpdateClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bMouceDown = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CAutoUpdateClientDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialog::OnNcHitTest( point );//把CTranDialog改成你的基类 
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest; 
}


void CAutoUpdateClientDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


BOOL CAutoUpdateClientDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreCreateWindow(cs);
}

void CAutoUpdateClientDlg::AniTick()
{
	m_AniLock.Lock();

	m_AniFrame += 1;
	if(m_AniFrame >= 30)
		m_AniFrame = 0;

	m_AniLock.Unlock();
}