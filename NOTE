// https://playground.arduino.cc/Code/BitMath/
// http://jscolor.com/examples/

#include <Adafruit_NeoPixel.h>

#define PIN 13
int LedBrightness = 255;

#define FontHeight 12

const uint16_t Zpix[] =
{
  0x04, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x20, 0x80, 0x20, 0x80, 0x10, 0x80, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x02, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 立
  0x08, 0x00, 0x08, 0x80, 0x10, 0x40, 0x23, 0xA0, 0xFC, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x20, 0x21, 0x20, 0xC1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 允
  0x41, 0xE0, 0x7D, 0x20, 0x91, 0x20, 0x7D, 0x20, 0x29, 0xE0, 0xC4, 0x00, 0x7F, 0xC0, 0x40, 0x40, 0x7F, 0xC0, 0x40, 0x40, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 智
  0x8A, 0x00, 0x4F, 0xC0, 0x12, 0x00, 0x02, 0x00, 0xDF, 0xC0, 0x40, 0x00, 0x5F, 0xC0, 0x50, 0x40, 0x5F, 0xC0, 0x40, 0x00, 0xBF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 造

  //  0x04, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0xBF, 0xA0, 0x20, 0x80, 0x3F, 0x80, 0x20, 0x00, 0x3F, 0xC0, 0x20, 0x40, 0x20, 0x40, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 官
  //  0x04, 0x00, 0xFF, 0xE0, 0x84, 0x20, 0xBF, 0xA0, 0x04, 0x00, 0xFF, 0xE0, 0x4A, 0x40, 0x7F, 0xC0, 0x04, 0x00, 0x52, 0xA0, 0x9F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 憲
  //  0x0F, 0x00, 0xE2, 0x80, 0x55, 0x40, 0x4B, 0xA0, 0xE0, 0x00, 0x4E, 0xA0, 0x4A, 0xA0, 0x6F, 0x40, 0xCB, 0x40, 0x0E, 0xA0, 0x0A, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 瑜
  //
  //  0x7F, 0xE0, 0x40, 0x20, 0x55, 0x20, 0x5F, 0xA0, 0x55, 0x20, 0x5F, 0xA0, 0x40, 0x20, 0x5F, 0xA0, 0x50, 0xA0, 0x9F, 0xA0, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 周
  //  0x08, 0x00, 0x08, 0x80, 0x10, 0x40, 0x23, 0xA0, 0xFC, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x20, 0x21, 0x20, 0xC1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 允
  //  0xFF, 0xC0, 0x08, 0x40, 0x08, 0x40, 0xFF, 0xE0, 0x08, 0x40, 0x08, 0x40, 0xFF, 0xC0, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 尹
  //
  //  0x5F, 0x80, 0x50, 0x80, 0x5F, 0x80, 0x40, 0x00, 0x7F, 0x80, 0x04, 0x80, 0xFF, 0xE0, 0x04, 0x00, 0x0A, 0x00, 0x11, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 吳
  //  0x04, 0x00, 0x04, 0x00, 0xFF, 0xE0, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0xFF, 0xE0, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 中
  //  0x20, 0x00, 0x2F, 0xC0, 0x40, 0x00, 0x40, 0x00, 0xC0, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x5F, 0xE0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 仁
  //
  //  0xE1, 0x00, 0xAF, 0xE0, 0xA1, 0x00, 0xAF, 0xE0, 0xC9, 0x20, 0xAF, 0xE0, 0xA9, 0x20, 0xAF, 0xE0, 0xE5, 0x40, 0x85, 0x40, 0x89, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 陳
  //  0x41, 0x40, 0x5F, 0xE0, 0x41, 0x00, 0x4F, 0xE0, 0xE9, 0x20, 0x4F, 0xE0, 0x49, 0x20, 0x4F, 0xE0, 0x40, 0x40, 0x5F, 0xE0, 0x55, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 博
  //  0x04, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0x80, 0x20, 0x7F, 0xC0, 0x04, 0x00, 0x04, 0x00, 0xFF, 0xE0, 0x04, 0x00, 0x04, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 宇
  //
  //  0x27, 0xE0, 0xF9, 0x20, 0x21, 0x20, 0xFA, 0x40, 0xAB, 0xE0, 0xAB, 0xE0, 0xFA, 0x20, 0x23, 0xE0, 0x72, 0x20, 0x6B, 0xE0, 0xA1, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 賴
  //  0x7F, 0xC0, 0x55, 0x40, 0x55, 0x40, 0x7F, 0xC0, 0x55, 0x40, 0x55, 0x40, 0x7F, 0xC0, 0x04, 0x00, 0x52, 0x40, 0x50, 0xA0, 0x9F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 思
  //  0x7F, 0xC0, 0x55, 0x40, 0x5F, 0x40, 0x55, 0x40, 0x4A, 0x40, 0x51, 0x40, 0x7F, 0xC0, 0x00, 0x00, 0x54, 0x40, 0x52, 0xA0, 0x9F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 恩
  //
  //  0x14, 0x00, 0x27, 0xE0, 0x69, 0x20, 0xA1, 0x00, 0x25, 0x40, 0x29, 0x20, 0x33, 0x20, 0x00, 0x00, 0x54, 0x40, 0x52, 0xA0, 0x9F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 您
  //  0x04, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0x08, 0x00, 0xFF, 0xE0, 0x10, 0x80, 0x20, 0x80, 0x79, 0x00, 0x06, 0x00, 0x19, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 安
  //  0x43, 0xE0, 0x40, 0x20, 0xF8, 0x40, 0x48, 0x80, 0x48, 0x80, 0x4B, 0xE0, 0x50, 0x80, 0x50, 0x80, 0x20, 0x80, 0x50, 0x80, 0x89, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 好
  //
  //  0x29, 0x00, 0x45, 0x20, 0xFF, 0x20, 0x55, 0x40, 0x6D, 0x40, 0x45, 0x20, 0x7D, 0x20, 0x11, 0x20, 0x7D, 0xC0, 0x29, 0x00, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 鄭
  //  0xF2, 0x40, 0x07, 0xE0, 0x02, 0x40, 0xFB, 0xC0, 0x22, 0x40, 0xAB, 0xC0, 0xAA, 0x40, 0xAF, 0xE0, 0xA8, 0x00, 0x22, 0x40, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 祺
  //  0x7C, 0x80, 0x10, 0x80, 0xFF, 0xE0, 0x10, 0xA0, 0xFE, 0xA0, 0xBA, 0xA0, 0x92, 0xA0, 0xFE, 0xA0, 0x11, 0x20, 0xFF, 0x20, 0xAA, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 勳
  //
  //  0x27, 0xC0, 0x24, 0x40, 0xF7, 0xC0, 0x24, 0x40, 0x2F, 0xE0, 0x74, 0x00, 0x67, 0xE0, 0xAA, 0xA0, 0x22, 0xA0, 0x25, 0x20, 0x2A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 楊
  //  0x00, 0xC0, 0x7F, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFF, 0xE0, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 千
  //  0x24, 0x40, 0x2F, 0xE0, 0x41, 0x00, 0x4F, 0xE0, 0xC1, 0x00, 0x4F, 0xE0, 0x45, 0x40, 0x5F, 0xE0, 0x45, 0x40, 0x5E, 0xA0, 0x4D, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 儀
  //
  //  0x20, 0x80, 0x20, 0x80, 0xFB, 0xE0, 0x20, 0x80, 0x20, 0x80, 0x71, 0xC0, 0x69, 0xC0, 0xA2, 0xA0, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 林
  //  0x7F, 0xC0, 0x08, 0x40, 0xFF, 0xE0, 0x08, 0x40, 0x7F, 0xC0, 0x10, 0x00, 0x20, 0x00, 0x7F, 0xC0, 0xA0, 0x40, 0x20, 0x40, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 君
  //  0x2F, 0xE0, 0x21, 0x00, 0x4F, 0xE0, 0x49, 0x20, 0xCD, 0x60, 0x40, 0x00, 0x4F, 0xE0, 0x41, 0x00, 0x4F, 0xE0, 0x4A, 0xA0, 0x4A, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 儒
  //
  //  0x22, 0x00, 0x4B, 0xC0, 0xFE, 0x20, 0x4B, 0xE0, 0x7A, 0x00, 0x4B, 0xC0, 0x7A, 0x20, 0x4B, 0xE0, 0x00, 0x00, 0x52, 0x40, 0x89, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 熊
  //  0x55, 0x80, 0xFE, 0x80, 0x45, 0xE0, 0xFE, 0xA0, 0x92, 0xA0, 0xFE, 0xA0, 0x10, 0xA0, 0xFE, 0xA0, 0x10, 0xA0, 0x1D, 0x20, 0xF1, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 勤
  //  0x04, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x20, 0x80, 0x20, 0x80, 0x10, 0x80, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x02, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 立
  //
  //  0xE7, 0xE0, 0x24, 0x00, 0x27, 0xE0, 0xE4, 0x00, 0x87, 0xE0, 0x84, 0x00, 0xEF, 0xE0, 0x24, 0x80, 0x24, 0xA0, 0x24, 0x40, 0xEF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 張
  //  0x04, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0xBF, 0xA0, 0x0C, 0x00, 0x73, 0x40, 0x0D, 0x80, 0x73, 0x80, 0x0D, 0x40, 0x71, 0x20, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 家
  //  0x04, 0x00, 0xFF, 0xE0, 0x40, 0x40, 0x7F, 0xC0, 0x00, 0x00, 0xFF, 0xE0, 0x94, 0x20, 0x66, 0xC0, 0x1B, 0x00, 0xE2, 0xE0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 豪

};

#define LedNumber 605
const uint16_t PixelMap[605] =
{
  0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
  109,  108,  107,  106,  105,  104,  103,  102,  101,  100,  99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,
  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,  128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
  219,  218,  217,  216,  215,  214,  213,  212,  211,  210,  209,  208,  207,  206,  205,  204,  203,  202,  201,  200,  199,  198,  197,  196,  195,  194,  193,  192,  191,  190,  189,  188,  187,  186,  185,  184,  183,  182,  181,  180,  179,  178,  177,  176,  175,  174,  173,  172,  171,  170,  169,  168,  167,  166,  165,
  220,  221,  222,  223,  224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,  247,  248,  249,  250,  251,  252,  253,  254,  255,  256,  257,  258,  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,  274,
  329,  328,  327,  326,  325,  324,  323,  322,  321,  320,  319,  318,  317,  316,  315,  314,  313,  312,  311,  310,  309,  308,  307,  306,  305,  304,  303,  302,  301,  300,  299,  298,  297,  296,  295,  294,  293,  292,  291,  290,  289,  288,  287,  286,  285,  284,  283,  282,  281,  280,  279,  278,  277,  276,  275,
  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,  340,  341,  342,  343,  344,  345,  346,  347,  348,  349,  350,  351,  352,  353,  354,  355,  356,  357,  358,  359,  360,  361,  362,  363,  364,  365,  366,  367,  368,  369,  370,  371,  372,  373,  374,  375,  376,  377,  378,  379,  380,  381,  382,  383,  384,
  439,  438,  437,  436,  435,  434,  433,  432,  431,  430,  429,  428,  427,  426,  425,  424,  423,  422,  421,  420,  419,  418,  417,  416,  415,  414,  413,  412,  411,  410,  409,  408,  407,  406,  405,  404,  403,  402,  401,  400,  399,  398,  397,  396,  395,  394,  393,  392,  391,  390,  389,  388,  387,  386,  385,
  440,  441,  442,  443,  444,  445,  446,  447,  448,  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,  459,  460,  461,  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,  491,  492,  493,  494,
  549,  548,  547,  546,  545,  544,  543,  542,  541,  540,  539,  538,  537,  536,  535,  534,  533,  532,  531,  530,  529,  528,  527,  526,  525,  524,  523,  522,  521,  520,  519,  518,  517,  516,  515,  514,  513,  512,  511,  510,  509,  508,  507,  506,  505,  504,  503,  502,  501,  500,  499,  498,  497,  496,  495,
  550,  551,  552,  553,  554,  555,  556,  557,  558,  559,  560,  561,  562,  563,  564,  565,  566,  567,  568,  569,  570,  571,  572,  573,  574,  575,  576,  577,  578,  579,  580,  581,  582,  583,  584,  585,  586,  587,  588,  589,  590,  591,  592,  593,  594,  595,  596,  597,  598,  599,  600,  601,  602,  603,  604,

};



Adafruit_NeoPixel ws2812b = Adafruit_NeoPixel(LedNumber, PIN, NEO_GRB + NEO_KHZ800);

int HAHA = 0;

void setup()
{
  Serial.begin(115200);
  LedInitialized();

  //  Animation_UpAndDownDurLeftAndRight(1, 39, 174, 232, 1, 217, 83, 24);
}

void loop()
{
#define Spacer 2

  for (int i = 0 ; i < 4; i++)
  {
    UnicodeChaDisplay(i, (FontHeight + Spacer) * (i), 0, 39, 174, 232, 0);
//    LedAllOff();
//    delay(500);
  }






  //#define Spacer 2
  //  randomSeed(analogRead(A0));
  //
  //  for (int i = 0; i < 33; i++)
  //  {
  //    switch (i % 3)
  //    {
  //      case 0:
  //        switch (map(random(0, 1023), 0, 1023, 0, 3))
  //        {
  //          case 0:
  //            Animation_LeftAndRight(1, 217, 83, 24);
  //            break;
  //
  //          case 1:
  //            Animation_UpAndDown(1, 39, 174, 232);
  //            break;
  //
  //          case 2:
  //            theaterChase(ws2812b.Color(map(random(0, 1023), 0, 1023, 0, 255), map(random(0, 1023), 0, 1023, 0, 255), map(random(0, 1023), 0, 1023, 0, 255)), 50); // Random
  //            break;
  //        }
  //        LedAllOff();
  //        UnicodeChaDisplay(i, (FontHeight + Spacer) * (i % 3), 0, 39, 174, 232, HAHA % 2);
  //        break;
  //
  //      case 1:
  //        UnicodeChaDisplay(i, (FontHeight + Spacer) * (i % 3), 0, 39, 174, 232, HAHA % 2);
  //        break;
  //
  //      case 2:
  //        UnicodeChaDisplay(i, (FontHeight + Spacer) * (i % 3), 0, 39, 174, 232, HAHA % 2);
  //        break;
  //    }
  //    delay(250);
  //  }
  //
  //  if (HAHA < 100)
  //  {
  //    HAHA++;
  //  }
  //  else
  //  {
  //    HAHA = 0;
  //  }

}
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
void TestAnimation()
{
  // Some example procedures showing how to display to the pixels:
  colorWipe(ws2812b.Color(255, 0, 0), 50); // Red
  colorWipe(ws2812b.Color(0, 255, 0), 50); // Green
  colorWipe(ws2812b.Color(0, 0, 255), 50); // Blue
  //colorWipe(ws2812b.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(ws2812b.Color(127, 127, 127), 50); // White
  theaterChase(ws2812b.Color(127, 0, 0), 50); // Red
  theaterChase(ws2812b.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
// 建議是275:1(上下:左右)
void Animation_UpAndDownDurLeftAndRight(unsigned long UDDetectInterval, int UDRed, int UDGreen, int UDBlue, unsigned long LRDetectInterval, int LRRed, int LRGreen, int LRBlue)
{
  Animation_LeftAndRight(LRDetectInterval, LRRed, LRGreen, LRBlue);
  Animation_UpAndDown(UDDetectInterval, UDRed, UDGreen, UDBlue);
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
unsigned long startTimeYUD;
int YUD = 0, Ydir = 1, YMin = 0, YMax = 10, Ystate = 1;
void Animation_UpAndDown(unsigned long DetectInterval, int Red, int Green, int Blue)
{
  //  RowDisplay(0, YUD, 1, Red, Green, Blue);
  //
  //  if ((millis() - startTimeYUD) >= DetectInterval)
  //  {
  //    startTimeYUD = millis();
  //
  //    if (YUD >= YMax)
  //    {
  //      Ydir = -1;
  //    }
  //    else if (YUD <= YMin)
  //    {
  //      Ydir = 1;
  //    }
  //    else
  //    {
  //      // do nothing
  //    }
  //    //
  //    //
  //    YUD += Ydir * 1;
  //
  //    RowDisplay(0, YUD - 1, 0, Red, Green, Blue); // 把上行清掉
  //    RowDisplay(0, YUD + 1, 0, Red, Green, Blue); // 把下行清掉
  //  }

  for (int i = 0; i < 11; i++)
  {
    RowDisplay(0, i, 1, Red, Green, Blue);
    delay(DetectInterval);
    RowDisplay(0, i, 0, Red, Green, Blue);
    delay(DetectInterval);
  }
  for (int i = 10; i >= 0; i--)
  {
    RowDisplay(0, i, 1, Red, Green, Blue);
    delay(DetectInterval);
    RowDisplay(0, i, 0, Red, Green, Blue);
    delay(DetectInterval);
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
unsigned long startTimeXLR;
int XLR = 0, Xdir = 1, XMin = 0, XMax = 43, Xstate = 1;
void Animation_LeftAndRight(unsigned long DetectInterval, int Red, int Green, int Blue)
{
  //  ColumeDisplay(0, XLR, 1, Red, Green, Blue);
  //
  //  if ((millis() - startTimeXLR) >= DetectInterval)
  //  {
  //    startTimeXLR = millis();
  //
  //    if (XLR >= XMax)
  //    {
  //      Xdir = -1;
  //    }
  //    else if (XLR <= XMin)
  //    {
  //      Xdir = 1;
  //    }
  //    else
  //    {
  //      // do nothing
  //    }
  //    //
  //    //
  //    XLR += Xdir * 1;
  //
  //    ColumeDisplay(0, XLR - 1, 0, Red, Green, Blue); // 把左列清掉
  //    ColumeDisplay(0, XLR + 1, 0, Red, Green, Blue); // 把右列清掉
  //  }

  for (int i = 0; i < 55; i++)
  {
    ColumeDisplay(0, i, 1, Red, Green, Blue);
    delay(DetectInterval);
    ColumeDisplay(0, i, 0, Red, Green, Blue);
    delay(DetectInterval);
  }
  for (int i = 43; i >= 0; i--)
  {
    ColumeDisplay(0, i, 1, Red, Green, Blue);
    delay(DetectInterval);
    ColumeDisplay(0, i, 0, Red, Green, Blue);
    delay(DetectInterval);
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void RowDisplay(int Addr, long WhichRow, boolean State, int Red, int Green, int Blue)  // Addr 功能未加入
{
  if (WhichRow < 11)
  {
    for (int i = 0; i < 55; i++)
    {
      if (State == 1)
      {
        ws2812b.setPixelColor(PixelMap[55 * WhichRow + i], Red, Green, Blue);
      }
      else
      {
        ws2812b.setPixelColor(PixelMap[55 * WhichRow + i], 0);
      }
    }
    ws2812b.show();
  }
  else
  {
    Serial.println(F("Over Row definition!"));// do nothing
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void ColumeDisplay(int Addr, long WhichCol, boolean State, int Red, int Green, int Blue)
{
  if (WhichCol < 43 + 55 * Addr + 1)
  {
    for (int i = 0; i < 11; i++)
    {
      if (State == 1)
      {
        ws2812b.setPixelColor(PixelMap[55 * i + WhichCol], Red, Green, Blue);
      }
      else
      {
        ws2812b.setPixelColor(PixelMap[55 * i + WhichCol], 0);
      }
    }
    ws2812b.show();
  }
  else
  {
    Serial.println(F("Over Column definition!"));// do nothing
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void UnicodeChaDownScroll(int WhichLetter, int CorX , int CorY, int Speed, int Red, int Green, int Blue) // 起點在左上方(0,0)
{
  for (int i = 0; i < 8; i++)
  {
    UnicodeChaDisplay(WhichLetter, CorX, CorY + i, Red, Green, Blue, 0);
    delayMiniSeconds(Speed);
    LedAllOff();
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void UnicodeChaDisplay(int WhichLetter, int CorX , int CorY, int Red, int Green, int Blue, boolean Bitshift) // 起點在左上方(0,0)
{
  switch (Bitshift)
  {
    case 0:
      for (byte x = 0; x < ((FontHeight - 1) * 2) - CorY * 2; ++x) // 12 Pixel HIGHT(11+11) & (假設CorY>0)修正超出顯示範圍的"列"(- CorY)
      {
        byte data = pgm_read_byte (&Zpix[x + WhichLetter * 32]); // fetch data from program memory
        // Serial.print("0x");
        // Serial.print(Zpix[x], HEX);
        // Serial.print(", ");
        for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
        {
          int EvenNumber = PixelMap[(CorX + CorY * 55) + (7 - y) + 55 * (x / 2)]; // 偶數列
          int OddNumber = PixelMap[(CorX + CorY * 55) + 8 + (7 - y) + 55 * (x / 2)]; // 奇數列

          if (x % 2 == 0) // 偶數(HEX左半)
          {
            // Serial.print(EvenNumber); Serial.print(", ");
            if ((data >> y) & 0x01)
            {
              // Serial.print("1"); // turn on the LED at location (x,y)
              ws2812b.setPixelColor(EvenNumber, Red, Green, Blue);
            }
            else
            {
              // Serial.print("0"); // turn off the LED at location (x,y)
              ws2812b.setPixelColor(EvenNumber, 0);
            }
          }
          else // 奇數(HEX右半)
          {
            // Serial.print(OddNumber); Serial.print(", ");
            if ((data >> y) & 0x01)
            {
              // Serial.print("1"); // turn on the LED at location (x,y)
              ws2812b.setPixelColor(OddNumber, Red, Green, Blue);
            }
            else
            {
              // Serial.print("0"); // turn off the LED at location (x,y)
              ws2812b.setPixelColor(OddNumber, 0);
            }
          }
        }
        ws2812b.show();
        Serial.println("");
      }
      break;

    case 1:
      // 左半先做
      for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
      {
        for (int x = 0; x < (FontHeight - 1); ++x) // 偶數先列出
        {
          byte data = pgm_read_byte (&Zpix[2 * x + (32 * WhichLetter + 0)]); // fetch data from program memory
          //          Serial.print("0x");
          //          Serial.print(Zpix[2 * x + (32 * WhichLetter + 0)], HEX);
          //          Serial.print(", ");

          if ((data >> y) & 0x01) // 直的一行讀下來，判斷是1還是0
          {
            //            Serial.print("1, ");
            //            Serial.println(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y)]);
            ws2812b.setPixelColor(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y)] , Red, Green, Blue);
          }
          else
          {
            //            Serial.print("0, ");
            //            Serial.println(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y)]);
            ws2812b.setPixelColor(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y)] , 0);
          }
        }
        ws2812b.show();
        Serial.println("");
      }
      // 再做右半
      for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
      {
        for (int x = 0; x < (FontHeight - 1); ++x) // 偶數先列出
        {
          byte data = pgm_read_byte (&Zpix[(2 * x + 1) + (32 * WhichLetter + 0)]); // fetch data from program memory
          //          Serial.print("0x");
          //          Serial.print(Zpix[(2 * x + 1) + (32 * WhichLetter + 0)], HEX);
          //          Serial.print(", ");

          if ((data >> y) & 0x01) // 直的一行讀下來，判斷是1還是0
          {
            //            Serial.print("1, ");
            //            Serial.println(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y) + 8]);
            ws2812b.setPixelColor(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y) + 8] , Red, Green, Blue);
          }
          else
          {
            //            Serial.print("0, ");
            //            Serial.println(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y) + 8]);
            ws2812b.setPixelColor(PixelMap[(CorX + CorY * 55) + (55 * x + 0) + (7 - y) + 8] , 0);
          }
        }
        ws2812b.show();
        Serial.println("");
      }
      break;
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < ws2812b.numPixels(); i++)
  {
    ws2812b.setPixelColor(i, c);
    ws2812b.show();
    delay(wait);
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < ws2812b.numPixels(); i++)
    {
      ws2812b.setPixelColor(i, Wheel((i + j) & 255));
    }
    ws2812b.show();
    delay(wait);
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < ws2812b.numPixels(); i++)
    {
      ws2812b.setPixelColor(i, Wheel(((i * 256 / ws2812b.numPixels()) + j) & 255));
    }
    ws2812b.show();
    delay(wait);
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
  for (int j = 0; j < 10; j++) // do 10 cycles of chasing
  {
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < ws2812b.numPixels(); i = i + 3)
      {
        ws2812b.setPixelColor(i + q, c); // turn every third pixel on
      }
      ws2812b.show();

      delay(wait);

      for (uint16_t i = 0; i < ws2812b.numPixels(); i = i + 3)
      {
        ws2812b.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
  for (int j = 0; j < 256; j++) // cycle all 256 colors in the wheel
  {
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < ws2812b.numPixels(); i = i + 3)
      {
        ws2812b.setPixelColor(i + q, Wheel( (i + j) % 255)); // turn every third pixel on
      }
      ws2812b.show();

      delay(wait);

      for (uint16_t i = 0; i < ws2812b.numPixels(); i = i + 3)
      {
        ws2812b.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return ws2812b.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return ws2812b.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ws2812b.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
void LedAllOff()
{
  for (int i = 0; i < LedNumber; i++)
  {
    ws2812b.setPixelColor(i, 0);
  }
  ws2812b.show();
}
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
void LedInitialized()
{
  ws2812b.begin();
  ws2812b.show(); // Initialize all pixels to 'off'
  ws2812b.setBrightness(LedBrightness);
  LedAllOff();

  for (int i = 0; i < LedNumber; i++)  // red
  {
    ws2812b.setPixelColor(i, 255, 0, 0);
  }
  ws2812b.show();
  delay(500);

  for (int i = 0; i < LedNumber; i++)  // green
  {
    ws2812b.setPixelColor(i, 0, 255, 0);
  }
  ws2812b.show();
  delay(500);

  for (int i = 0; i < LedNumber; i++)  // blue
  {
    ws2812b.setPixelColor(i, 0, 0, 255);
  }
  ws2812b.show();
  delay(500);

  for (int i = 0; i < LedNumber; i++)  // white
  {
    ws2812b.setPixelColor(i, 255, 255, 255);
  }
  ws2812b.show();
  delay(500);
  //
  LedAllOff();
}
/*---------------------------------------To defeat the watch dog----------------------------------------------------------------------------------------*/
void delayMiniSeconds(int miniseconds) {
  for (int i = 0; i <= miniseconds; i++) {
    delay(1);
    //    ESP.wdtFeed();
  }
}
