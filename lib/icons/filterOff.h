#include <Arduino.h>
const int32_t filterOff_width = 43;
const int32_t filterOff_height = 120;
const uint8_t filterOff_data[(44*120)/2] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFE, 0xFF, 0xDF, 0xFE, 0xFF, 0xEE, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xBB, 0xBB, 0xBB, 0x9B, 0xCA, 0xCD, 0xAA, 0xBB, 0xBB, 0xBA, 0xBB, 0xEC, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xAF, 0xA9, 0xAA, 0x99, 0xAA, 0xAA, 0x9A, 0xAA, 0x99, 0xAA, 0x9A, 0x99, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBA, 0xFE, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xDF, 0xBB, 0xBB, 0xBB, 0x9B, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xDA, 0x0F, 
	0xCF, 0xAA, 0x99, 0xA9, 0xAB, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x9A, 0xAA, 0x99, 0x99, 0xC9, 0x0F, 
	0xCF, 0xAA, 0x99, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0x99, 0xCA, 0x0F, 
	0xCF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0x0F, 
	0xCF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0x0F, 
	0xCF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0x0F, 
	0xBF, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0x0F, 
	0xEF, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xEC, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xFE, 0xFF, 0x0F, 
	0xFF, 0xFF, 0x9A, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0x9A, 0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0xFC, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0x99, 0xA9, 0x89, 0xA9, 0xAA, 0xAA, 0xAA, 0x99, 0xAA, 0xAA, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xBA, 0xBB, 0xCB, 0x9A, 0x9A, 0xAA, 0xBB, 0xAA, 0xAA, 0xAA, 0xBB, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xBA, 0xFE, 0xDE, 0xAB, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAB, 0xA9, 0x99, 0xA9, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0x9A, 0xA9, 0xAA, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xBE, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xCA, 0xAB, 0xA9, 0x99, 0xCA, 0xCD, 0xAA, 0xAA, 0xFC, 0xFF, 0xDE, 0xAB, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0x9C, 0xAA, 0xED, 0xFF, 0xFF, 0xAC, 0xAA, 0xCA, 0xBC, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xCA, 0xAB, 0xCA, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xAA, 0x99, 0xA9, 0xAA, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xBA, 0xAA, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0xAA, 0x9A, 0xA9, 0xBA, 0xAA, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF, 0xAE, 0xAA, 0xAA, 0xDB, 0xFE, 0xAC, 0xA9, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0xFD, 0xFF, 0xFF, 0xCF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x9D, 0xA8, 0xFE, 0xFF, 0xFF, 0xDF, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0xB9, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0x9A, 0xA9, 0xBA, 0xFE, 0xFF, 0xFF, 0xEF, 0x9A, 0xCA, 0xFF, 0xFF, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xBA, 0xAB, 0xAA, 0xEA, 0xFF, 0xFF, 0xBE, 0x9A, 0xD9, 0xFF, 0xFF, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xBE, 0xAA, 0xBA, 0xDD, 0xAB, 0x99, 0xAA, 0xDA, 0xFF, 0xFF, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xDF, 0xAB, 0xAA, 0xA9, 0x9A, 0xAA, 0xAA, 0xA9, 0xFD, 0xFF, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xBE, 0xAA, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0xDB, 0xFF, 0xFF, 0xDF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xDF, 0xAA, 0x9A, 0xBA, 0xED, 0xBE, 0xA9, 0xAA, 0xFE, 0xDE, 0xBC, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xDF, 0xAA, 0xDA, 0xFF, 0xFF, 0xEF, 0xAB, 0xAA, 0xBA, 0xAA, 0xAA, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xBF, 0xA9, 0xEB, 0xFF, 0xFF, 0xFF, 0xBE, 0xA9, 0xAA, 0xAB, 0x99, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xBE, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xEF, 0xAB, 0xAA, 0xAA, 0xBA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xAD, 0x9A, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0xDB, 0xDE, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xDA, 0xFF, 0x9C, 0xAA, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xAB, 0xBA, 0xFF, 0xCF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xBA, 0xAB, 0xBA, 0xBA, 0xFE, 0xFF, 0xFF, 0xFF, 0xEF, 0xAB, 0xCA, 0xFF, 0xDF, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xAA, 0xA9, 0xAA, 0xBA, 0xFC, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xDA, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0x99, 0xAA, 0x9A, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xEB, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xCB, 0xDD, 0xAA, 0xAA, 0xFC, 0xFF, 0xDE, 0xBC, 0xAA, 0xFC, 0xFF, 0xCF, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xAD, 0x9A, 0xBA, 0xBC, 0xAA, 0xAA, 0x9A, 0xEB, 0xFF, 0xCF, 0x99, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xDF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBA, 0xFE, 0xCF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0x9A, 0x99, 0xBA, 0xAB, 0x9A, 0xEB, 0xDF, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xAE, 0xAA, 0xBA, 0xEC, 0xFF, 0xBE, 0x9A, 0xAA, 0xCE, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xAE, 0x9A, 0xFC, 0xFF, 0xFF, 0xEF, 0x9A, 0xAA, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xDA, 0xFF, 0xFF, 0xFF, 0x9D, 0xAA, 0xFD, 0xFF, 0xFF, 0xFF, 0xBE, 0x9A, 0x98, 0x89, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xD9, 0xFF, 0xFF, 0xFF, 0x9C, 0xAA, 0xFE, 0xFF, 0xFF, 0xFF, 0xEF, 0xAB, 0x8A, 0x98, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xAB, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xAA, 0x99, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xDA, 0xFF, 0xFF, 0xBE, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xBB, 0x99, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xB9, 0xDC, 0xBC, 0x99, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xAA, 0xCB, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xA9, 0x9A, 0x9A, 0xA9, 0xFC, 0xFF, 0xFF, 0xFF, 0x9E, 0x99, 0xCC, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0x99, 0xAB, 0x99, 0xA9, 0xAB, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xAD, 0xA9, 0xDD, 0xA9, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0x9A, 0xAA, 0xBA, 0xEC, 0xCF, 0xBA, 0x9A, 0xFC, 0xDF, 0xAC, 0xAA, 0xA9, 0xDE, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0x9A, 0xCA, 0xFF, 0xFF, 0xFF, 0x9C, 0xAA, 0xBA, 0x9A, 0xAA, 0xAA, 0x9A, 0xCC, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0x99, 0xEB, 0xFF, 0xFF, 0xFF, 0xBF, 0xAA, 0xAA, 0xAA, 0x99, 0xAA, 0x9A, 0xAA, 0x99, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xAC, 0xA9, 0xAA, 0xAA, 0xCC, 0xAA, 0x9A, 0x9A, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xA9, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0xDC, 0xFE, 0xFF, 0xAD, 0x99, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0x9C, 0xA9, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xFD, 0xFF, 0xFF, 0xFF, 0xEF, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0x9A, 0xEA, 0xFF, 0xFF, 0xFF, 0xEF, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xBA, 0xFE, 0xFF, 0xFF, 0xCF, 0x9A, 0xEA, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xEB, 0xFF, 0xDE, 0xAB, 0xA9, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xAE, 0xAA, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xAA, 0xAA, 0xBA, 0xBC, 0x9A, 0x99, 0xAA, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 0x9D, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xCB, 0xAB, 0x9A, 0xA9, 0x9A, 0xAA, 0x9A, 0x99, 0xFD, 0xFF, 0xFF, 0xFF, 0xAC, 0x99, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDB, 0xBE, 0xAA, 0xAA, 0xAA, 0xCB, 0xAB, 0xAA, 0xDB, 0xFF, 0xFF, 0xDF, 0x9A, 0x99, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xCF, 0xAA, 0xBA, 0xDC, 0xFF, 0xBE, 0x9A, 0xAA, 0xED, 0xAC, 0x99, 0xA9, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xBF, 0xA9, 0xFC, 0xFF, 0xFF, 0xEF, 0x9B, 0xAA, 0xAA, 0x99, 0xA9, 0xAA, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xD9, 0xAE, 0xA9, 0xFC, 0xFF, 0xFF, 0xFF, 0xBE, 0xAA, 0x9A, 0xA9, 0xAA, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xAD, 0xAA, 0xFD, 0xFF, 0xFF, 0xFF, 0xEF, 0xAB, 0xAA, 0xBA, 0xDC, 0xCE, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xDA, 0xAC, 0xBA, 0xFE, 0xFF, 0xFF, 0xFF, 0xEF, 0xAB, 0xCA, 0xFE, 0xFF, 0xCF, 0xAA, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xB9, 0x9A, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xDA, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xA9, 0xAA, 0xB9, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0x9A, 0xEB, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x8A, 0xA9, 0xAA, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xBF, 0xAA, 0xFB, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xAD, 0xA9, 0xFC, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xDA, 0xDF, 0xAA, 0xAA, 0xEC, 0xDE, 0xAC, 0xAA, 0xAA, 0xFD, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xAD, 0xAA, 0xBA, 0xAB, 0xAA, 0xA9, 0xAA, 0xEB, 0xFF, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xDF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBA, 0xAA, 0xFE, 0xFF, 0xCF, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0x9D, 0xAA, 0xAA, 0x99, 0xDC, 0xAB, 0xAB, 0xEB, 0xFF, 0xCF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xBE, 0xAA, 0xDB, 0xFE, 0xFF, 0xAE, 0xBA, 0xAA, 0xCC, 0xAB, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xAD, 0xAA, 0xFD, 0xFF, 0xFF, 0xEF, 0xAB, 0xAA, 0x9A, 0xAA, 0x9A, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0x9B, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0xAA, 0xAA, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xEF, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0xA9, 0xBC, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xDF, 0xAA, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xA9, 0xEB, 0xCF, 0xAA, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0xAA, 0xDB, 0xDE, 0xBC, 0xAA, 0xEA, 0xFF, 0xFF, 0xFF, 0xFF, 0xAE, 0x99, 0xFB, 0xCF, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xA9, 0xAB, 0xAA, 0xAA, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xAE, 0x9A, 0xFC, 0xCF, 0xA9, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0xBA, 0xAA, 0xFC, 0xFF, 0xFF, 0xFF, 0xAD, 0xA9, 0xFE, 0xDF, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xA9, 0xBB, 0xAB, 0xAA, 0xBA, 0xFF, 0xFF, 0xDE, 0xAB, 0xBA, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xBA, 0xDC, 0xFE, 0xCF, 0xAA, 0xAA, 0xEC, 0xBD, 0xAA, 0xAA, 0xBA, 0xFF, 0xCF, 0x9A, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xAC, 0xAA, 0xAA, 0xAA, 0xAA, 0x99, 0xA9, 0xFC, 0xCF, 0x9A, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xCF, 0xAA, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xCA, 0xDE, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0xA9, 0xAA, 0xDB, 0xDE, 0xAA, 0xAA, 0xCC, 0xAA, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xAB, 0x9A, 0xED, 0xFF, 0xFF, 0xAD, 0xAA, 0xAA, 0xAB, 0x99, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xEF, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xAA, 0x99, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xDA, 0xFF, 0xFF, 0xFF, 0xDF, 0xAA, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xBA, 0xAB, 0xAA, 0xBA, 0x9B, 0x99, 0xBA, 0xAB, 0xAA, 0xAA, 0xBB, 0xBB, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0x99, 0xA9, 0x9A, 0x9A, 0x99, 0xAA, 0x99, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0xAA, 0xAA, 0x99, 0xA9, 0xAB, 0xAA, 0xA9, 0xA9, 0x99, 0x99, 0xA9, 0xAA, 0xAA, 0xAA, 0x9A, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAA, 0x9A, 0x9A, 0x9A, 0x88, 0x98, 0xAA, 0xA9, 0x9A, 0xAA, 0x89, 0x88, 0x99, 0x9A, 0xAA, 0xAA, 0xAA, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xAB, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0xFB, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xFE, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xEF, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xEC, 0x0F, 
	0xBF, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0xCA, 0x0F, 
	0xBF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xCA, 0x0F, 
	0xCF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xCA, 0x0F, 
	0xBF, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xCA, 0x0F, 
	0xBF, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xCA, 0x0F, 
	0xBF, 0x99, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB9, 0x0F, 
	0xDF, 0xBB, 0xBB, 0xBB, 0xAB, 0xA9, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xBA, 0xBB, 0xBB, 0xDA, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xAD, 0xAA, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0xAA, 0x99, 0xAA, 0xAA, 0xBA, 0xFE, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xAA, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0x99, 0xAA, 0xAA, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x9B, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xA9, 0xCB, 0xEE, 0xBD, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
	};