/*********************************************************************************************
* File��	AscII6x8.c
* Author:	embest	
* Desc��	Ascii table in 6x8 dot lattice 
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "def.h"

/*--- macro defines ---*/
#define Ascii_W 8
#define XWIDTH 6

/*--- global variables ---*/
/* ascii table */
const INT8U g_auc_Ascii6x8[]=
{
/* 0x0 */
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*0x10*/
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*0x20*/ 
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x1c,0x22,0x24,0x18,0x16,0x00,0x00, /*!:��*/
  0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00, /*"*/
  0x00,0x14,0x3e,0x14,0x3e,0x14,0x00,0x00, /* 0x00,0x02,0x04,0x78,0x10,0x0c,0x02,0x00,#:��*/
  0x00,0x7e,0x15,0x15,0x15,0x0a,0x00,0x00, /*$:��*/
  0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00, /*%*/
  0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00, /*&*/
  0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00, /*'*/
  0x00,0x00,0x1C,0x22,0x41,0x00,0x00,0x00, /*(*/
  0x00,0x00,0x41,0x22,0x1C,0x00,0x00,0x00, /*)*/
  0x00,0x14,0x08,0x3E,0x08,0x14,0x00,0x00, /***/
  0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00, /*+*/
  0x00,0x00,0x50,0x30,0x00,0x00,0x00,0x00, /*;*/
  0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00, /*-*/
  0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00, /*.*/
  0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00, /*/*/
/*0x30*/
  0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00, /*0*/
  0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00, /*1*/
  0x00,0x42,0x61,0x51,0x49,0x46,0x00,0x00, /*2*/
  0x00,0x21,0x41,0x45,0x4B,0x31,0x00,0x00, /*3*/
  0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00, /*4*/
  0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00, /*5*/
  0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00, /*6*/
  0x00,0x01,0x01,0x79,0x05,0x03,0x00,0x00, /*7*/
  0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00, /*8*/
  0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00, /*9*/
  0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00, /*:*/
  0x00,0x00,0x56,0x36,0x00,0x00,0x00,0x00, /*;*/
  0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00, /*<*/
  0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00, /*=*/
  0x00,0x00,0x41,0x22,0x14,0x08,0x00,0x00, /*>*/
  0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00, /*?*/
/* 0x40 */
  0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00, /*@*/
  0x00,0x7E,0x11,0x11,0x11,0x7E,0x00,0x00, /*A*/
  0x00,0x41,0x7F,0x49,0x49,0x36,0x00,0x00, /*B*/
  0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00, /*C*/
  0x00,0x41,0x7F,0x41,0x41,0x3E,0x00,0x00, /*D*/
  0x00,0x7F,0x49,0x49,0x49,0x49,0x00,0x00, /*E*/
  0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00, /*F*/
  0x00,0x3E,0x41,0x41,0x49,0x7A,0x00,0x00, /*G*/
  0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00, /*H*/
  0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x00, /*I*/
  0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00, /*J*/
  0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00, /*K*/
  0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00, /*L*/
  0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00, /*M*/
  0x00,0x7F,0x06,0x08,0x30,0x7F,0x00,0x00, /*N*/
  0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00, /*O*/
/* 0x50 */
  0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00, /*P*/
  0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00, /*Q*/
  0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00, /*R*/
  0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00, /*S*/
  0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00, /*T*/
  0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00, /*U*/
  0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00, /*V*/
  0x00,0x7F,0x20,0x18,0x20,0x7F,0x00,0x00, /*W*/
  0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00, /*X*/
  0x00,0x07,0x08,0x70,0x08,0x07,0x00,0x00, /*Y*/
  0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00, /*Z*/
  0x00,0x00,0x7F,0x41,0x41,0x00,0x00,0x00, /*[*/
  0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00, /*\*/
  0x00,0x00,0x41,0x41,0x7F,0x00,0x00,0x00, /*]*/
  0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00, /*^*/
  0x00,0x40,0x40,0x40,0x40,0x40,0x00,0x00, /*_*/
/* 0x60 */
  0x00,0x00,0x00,0x07,0x05,0x07,0x00,0x00, /*`*/
  0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00, /*a*/
  0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00, /*b*/
  0x00,0x38,0x44,0x44,0x44,0x28,0x00,0x00, /*c*/
  0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00, /*d*/
  0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00, /*e*/
  0x00,0x00,0x08,0x7E,0x09,0x02,0x00,0x00, /*f*/ 
  0x00,0x0C,0x52,0x52,0x4C,0x3E,0x00,0x00, /*g*/
  0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00, /*h*/
  0x00,0x00,0x44,0x7D,0x40,0x00,0x00,0x00, /*i*/
  0x00,0x20,0x40,0x44,0x3D,0x00,0x00,0x00, /*j*/
  0x00,0x00,0x7F,0x10,0x28,0x44,0x00,0x00, /*k*/
  0x00,0x00,0x41,0x7F,0x40,0x00,0x00,0x00, /*l*/
  0x00,0x7C,0x04,0x78,0x04,0x78,0x00,0x00, /*m*/
  0x00,0x7C,0x08,0x04,0x04,0x78,0x00,0x00, /*n*/
  0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00, /*o*/
/* 0x70 */
  0x00,0x7E,0x0C,0x12,0x12,0x0C,0x00,0x00, /*p*/
  0x00,0x0C,0x12,0x12,0x0C,0x7E,0x00,0x00, /*q*/
  0x00,0x7C,0x08,0x04,0x04,0x08,0x00,0x00, /*r*/
  0x00,0x58,0x54,0x54,0x54,0x64,0x00,0x00, /*s*/
  0x00,0x04,0x3F,0x44,0x40,0x20,0x00,0x00, /*t*/
  0x00,0x3C,0x40,0x40,0x3C,0x40,0x00,0x00, /*u*/
  0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00, /*v*/
  0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00, /*w*/
  0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00, /*x*/
  0x00,0x1C,0xA0,0xA0,0x90,0x7C,0x00,0x00, /*y*/
  0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00, /*z*/
  0x00,0x00,0x08,0x36,0x41,0x00,0x00,0x00, /*{*/
  0x00,0x00,0x00,0x77,0x00,0x00,0x00,0x00, /*|*/
  0x00,0x00,0x41,0x36,0x08,0x00,0x00,0x00, /*}*/
  0x00,0x08,0x04,0x08,0x10,0x08,0x00,0x00, /*~*/
  0x00,0x5c,0x62,0x02,0x62,0x5c,0x00,0x00  /*OM*/
};
