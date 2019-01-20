#include "FastLED.h"

// palettes
DEFINE_GRADIENT_PALETTE(black_gp){
    0, 0, 0, 0,    // full black
    255, 0, 0, 0}; // full black
CRGBPalette16 black_p = black_gp;

DEFINE_GRADIENT_PALETTE(white_gp){
    0, 255, 255, 255,    // full white
    255, 255, 255, 255}; // full white
CRGBPalette16 white_p = white_gp;

//mine demo test
DEFINE_GRADIENT_PALETTE(morning1_gp){
    0, 255, 0, 0,      // red
    255, 255, 255, 0}; // yellow
CRGBPalette16 morning1_p = morning1_gp;
DEFINE_GRADIENT_PALETTE(morning2_gp){
    0, 255, 0, 0,    // red
    255, 0, 0, 255}; // blue
CRGBPalette16 morning2_p = morning2_gp;

CRGB b = CRGB::Black;
CRGB star(20, 20, 20);
CRGBPalette16 nightPalette(b, b, b, star, b, b, b, star, b, b, b, star, b, b, b, star);

// below palletes based on https://css-tricks.com/radial-gradient-recipes/
/*
#sun {
  background: -webkit-radial-gradient(bottom, circle, rgba(242,248,247,1) 0%,rgba(249,249,28,1) 3%,rgba(247,214,46,1) 8%, rgba(248,200,95,1) 12%,rgba(201,165,132,1) 30%,rgba(115,130,133,1) 51%,rgba(46,97,122,1) 85%,rgba(24,75,106,1) 100%);
  opacity: 0.5;
*/
DEFINE_GRADIENT_PALETTE(sun_gp){
    0, 242, 248, 247,
    8, 249, 249, 28,    // 3%
    20, 247, 214, 46,   // 8%
    30, 248, 200, 95,   // 12%
    77, 201, 165, 132,  // 30%
    130, 115, 130, 133, // 51%
    217, 46, 97, 122,   // 85%
    255, 24, 75, 106};
CRGBPalette16 sun_p = sun_gp;

/*
#sunDay {
  background: -webkit-radial-gradient(bottom, circle, rgba(252,255,251,0.9) 0%,rgba(253,250,219,0.4) 30%,rgba(226,219,197,0.01) 70%, rgba(226,219,197,0.0) 70%,rgba(201,165,132,0) 100%);
 opacity: 0.5;
 */
DEFINE_GRADIENT_PALETTE(sunDay_gp){
    0, 252, 255, 251,
    77, 253, 250, 219,  // 30%
    179, 226, 219, 197, // 70%
    255, 201, 165, 132};
CRGBPalette16 sunDay_p = sunDay_gp;

/*
#moon {
  background: -webkit-radial-gradient(40% 55%, circle, rgba(249,249,250,1) -1%, rgba(189,255,254,1) 1%, rgba(8,49,78,1) 1%, rgba(8,26,56,1) 10%, rgba(4,16,46,1) 40%, rgba(2,8,13,1) 70%);
  opacity: 0;  
*/

/*
#sunSet {
    background: -webkit-radial-gradient(bottom, circle, rgba(254,255,255,0.8) 5%,rgba(236,255,0,1) 10%,rgba(253,50,41,1) 25%, rgba(243,0,0,1) 40%,rgba(93,0,0,1) 100%);
     opacity: 0.3;
*/
DEFINE_GRADIENT_PALETTE(sunSet_gp){
    0, 254, 255, 255,
    13, 254, 255, 255, // 5%
    26, 236, 255, 0,   // 10%
    64, 253, 50, 41,   // 25%
    102, 243, 0, 0,    // 40%
    255, 93, 0, 0};
CRGBPalette16 sunSet_p = sunSet_gp;

/*
#sky {
   background: -webkit-linear-gradient(bottom, rgba(249,251,240,1) 10%, rgba(215,253,254,1) 20%, rgba(167,222,253,1) 40%, rgba(110,175,255,1) 100%); 
   opacity: 0.52;
*/
DEFINE_GRADIENT_PALETTE(sky_gp){
    0, 249, 251, 240,
    26, 249, 251, 240,  // 10%
    51, 215, 253, 254,  // 20%
    102, 167, 222, 253, // 40%
    255, 110, 175, 255};
CRGBPalette16 sky_p = sky_gp;

/*
#horizonNight {
  background: -webkit-linear-gradient(bottom, rgba(57,167,255,1) 0%, rgba(13,98,245,1) 20%,rgba(0,11,22,0.1) 60%);
  opacity: 0;
*/
// reduced brightness by roughly 65%
DEFINE_GRADIENT_PALETTE(horizonNight_gp){
    0, 37, 108, 165, // 0%
    51, 8, 66, 166,  // 20%
    153, 0, 9, 18,   // 60%
    255, 0, 0, 0};
CRGBPalette16 horizonNight_p = horizonNight_gp;
