//
// 		interface.h
//

#ifndef apero_interface_h
#define apero_interface_h

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

#include "display.h"
#include "shapes.h"
#include "couleurs.h"

void OnQuitter(GtkWidget* widget, gpointer data);
void OnAbout(GtkWidget* widget, gpointer data);
void OnRadio(GtkWidget* widget, gpointer data);
void OnTearoff(GtkWidget* widget, gpointer data);
void OnCheck(GtkWidget* widget, gpointer data);
void file_selection_save();
void file_selection_load();
void recuperer_chemin(GtkWidget *bouton, GtkWidget *file_selection);
GtkWidget* CreateMenu(GtkWidget* widget);
GtkWidget* Create_toolbar();
//GtkWidget* sdl_surface_to_GtkWidget(SDL_Surface* img);
void *run(void *arg);
void Function(GtkWidget* widget, gpointer data);
void Undo();
void Redo();
void Clear();
void Palette();
void PickColor();
void Save(GtkWidget* widget, GtkWidget* file_selection);
void Open(GtkWidget* widget, GtkWidget* file_selection);
void quit();
void Rotate();
void rotation_horizontale(t_display display);
void rotation_verticale(t_display display);
void Fractales(GtkWidget* widget, gpointer data);
void ZoomIN();
void ZoomOUT();
void Lumi();
void OnScrollbarChange();

int 		sdl_loop(t_display *display);


void FilterBW();
SDL_Surface* filtre_noir_blanc(t_display display);
void FilterG();
SDL_Surface* filtre_gris(t_display display);
void FilterInv();
SDL_Surface* filtre_inverser(t_display display);
void FilterBlue();
SDL_Surface* filtre_bleu(t_display display);
void FilterGreen();
SDL_Surface* filtre_vert(t_display display);
void FilterRed();
SDL_Surface* filtre_rouge(t_display display);
void FilterLumPlus();
SDL_Surface* filtre_lum_plus(t_display display);
void FilterLumMoins();
SDL_Surface* filtre_lum_moins(t_display display);
void Detec();
SDL_Surface* SDL_DetectionsBords(t_display display);
void Repoussage();
SDL_Surface* SDL_Repoussage(t_display display);
void Flou();
SDL_Surface* SDL_Flou(t_display display);
void Gaufrage();
SDL_Surface* SDL_Gaufrage(t_display display);
void AugContraste();
SDL_Surface* SDL_AugmenterContraste(t_display display);
void FlouGaussien();
SDL_Surface* SDL_FlouGaussien(t_display display, float sigma, unsigned int taille_noyau);

SDL_Surface* filtre_bright_m(t_display display, int coef);
SDL_Surface* filtre_bright_p(t_display display, int coef);



	#endif
