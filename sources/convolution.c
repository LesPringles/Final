#include "../includes/display.h"
#include "../includes/couleurs.h"
#include <math.h>

typedef struct CouleurRGB
{
 Uint8 r;
 Uint8 g;
 Uint8 b;      
} Couleur;

typedef struct Matrice_
{
 float * m;
 int nblignes;
 int nbcolonnes;               
} Matrice;

static float maxi(float a, float b);
static float mini(float a, float b);

static Couleur couleurNoir();



/*Creer une couleur RGB*/
static Couleur couleurCreer(Uint8, Uint8, Uint8);

/* retourne la couleur multipliée par un flottant*/
//static Couleur couleurMult(Couleur couleur, float x);

/*Somme de deux couleurs*/
static Couleur couleurSomme(Couleur, Couleur);

/*inverse une couleur*/
//static Couleur couleurInverser(Couleur);


int matriceInitialiser(Matrice*, int nbligne, int nbcolonne);
void matriceEcrire(Matrice*, int ligne, int colonne, float element);
float matriceLire(Matrice*, int ligne, int colonne);

int matriceGetNbLignes(Matrice*);
int matriceGetNbColonnes(Matrice*);

static int matriceDetruire(Matrice* matrice);

float matriceCalculerSomme(Matrice* m);
void matriceDiviser(Matrice* m, float diviseur);

SDL_Surface* convolveNM(SDL_Surface* origine, Matrice* matrice);

////////////////// FIN HEADER //////////////////////////

float  mini(float a, float b)
{
 if (a<b)
  return a;
 else
  return b;    
}

float  maxi(float a, float b)
{
 if (a<b) 
  return b;
 else 
  return a;      
}

Couleur  couleurNoir()
{
 Couleur couleur;
 couleur.r = 0;
 couleur.g = 0;
 couleur.b = 0;
 return couleur;        
}



int matriceInitialiser(Matrice* matrice, int nbligne, int nbcolonne)
{
 float* m;
 int i;
 if(matrice==NULL)
  return EXIT_FAILURE;
 
 m = (float*) malloc(nbligne * nbcolonne * sizeof(float));
 
 if (m==NULL)
  return EXIT_FAILURE;
  
 matrice->m = m;
 matrice->nblignes = nbligne;
 matrice->nbcolonnes = nbcolonne;
 for(i=0;i<(nbligne*nbcolonne);i++)
  m[i] = 0.0F;
 
 return EXIT_SUCCESS;         
}

int matriceDetruire(Matrice* matrice)
{
 if (matrice->m ==NULL)
   return EXIT_FAILURE;

 free(matrice->m);
  
 return EXIT_SUCCESS;   
}

int matriceGetNbLignes(Matrice* matrice)
{
  return matrice->nblignes;
}

int matriceGetNbColonnes(Matrice* matrice)
{
   return matrice->nbcolonnes;      
}

void  matriceEcrire(Matrice* m, int ligne, int colonne, float element)
{
 if (ligne>0 && colonne>0 && ligne<=matriceGetNbLignes(m) && colonne<=matriceGetNbColonnes(m))
   m->m[(ligne-1) * matriceGetNbColonnes(m) + (colonne-1)] =element ;    
}

float  matriceLire(Matrice* m, int ligne, int colonne)
{
 return m->m[(ligne-1) * matriceGetNbColonnes(m) + (colonne-1)];     
}

float  matriceCalculerSomme(Matrice* m)
{
 int i;
 float res = 0.0F;
 
 for(i=0;i<m->nblignes*m->nbcolonnes;i++)
   res += m->m[i];

 return res;      
}

void matriceDiviser(Matrice* m, float diviseur)
{
 int i;
 
 for(i=0;i<m->nblignes*m->nbcolonnes;i++)
    m->m[i] = m->m[i]/diviseur;
}

Couleur  couleurCreer(Uint8 r, Uint8 g, Uint8 b)
{
 Couleur dest;
 dest.r = r;
 dest.g = g;
 dest.b = b;

 return dest;        
}

/*Couleur  couleurMult(Couleur couleur, float x)
{
 Couleur dest;
 dest.r = (Uint8) ((float) ((float) couleur.r) * x);
 dest.g = (Uint8) ((float) ((float) couleur.g) * x);
 dest.b = (Uint8) ((float) ((float) couleur.b) * x);
 return dest;         
          
} */

/*Couleur  couleurInverser(Couleur c)
{
 Couleur dest;
 dest.r = ((Uint8) 255) -c.r;
 dest.g = ((Uint8) 255) -c.g;
 dest.b = ((Uint8) 255) -c.b;
 
 return dest;
        
}*/
        

Couleur  couleurSomme(Couleur c1, Couleur c2)
{
 Couleur dest;
 dest.r = (Uint8) mini(255.0f, (float) c1.r + c2.r);
 dest.g = (Uint8) mini(255.0f, (float) c1.g + c2.g);
 dest.b = (Uint8) mini(255.0f, (float) c1.b + c2.b);
 
 return dest;        
}

/*Lit la couleur au format Uint32 d'une surface*/
static Uint32 SDL_LireCouleurAux(SDL_Surface* surface, int x, int y);

/*Lit une couleur au format Couleur d'une surface*/
static Couleur SDL_LireCouleur(SDL_Surface * surface, int x, int y);

/*Permet d'écrire sur une surface avec une couleur Uint32*/
static void SDL_EcrireCouleurAux(SDL_Surface* surface, int x, int y, Uint32 pixel);

/*Permet de lire sur une surface avec une couleur Couleur*/
static void SDL_EcrireCouleur(SDL_Surface* surface, int x, int y, Couleur couleur);


/*Détermine la couleur d'un pixel à la position x, y d'une surface soumis à la convolution d'une matrice*/
static Couleur couleurConvolutionNM(SDL_Surface* surface, int x, int y, Matrice* matrice);







/*
 *
 * Définition des opérations sur le type SDL_Surface
 *
 */

Uint32  SDL_LireCouleurAux(SDL_Surface* surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;

  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
             case 1:
                  return *p;
             case 2:
                  return *(Uint16 *)p;
             case 3:
                 if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                     return p[0] << 16 | p[1] << 8 | p[2];
                 else
                     return p[0] | p[1] << 8 | p[2] << 16;
             case 4:
                  return *(Uint32 *)p;
             default:
                  return 0;
  }        
}



Couleur  SDL_LireCouleur(SDL_Surface * surface, int x, int y)
{
 Couleur couleur;
 if ((x<0) | (y<0) | (x>=(surface->w)) | (y>=(surface->h)))
  return couleurNoir();
 
 SDL_GetRGB(SDL_LireCouleurAux(surface, x, y), surface->format, 
                                      (Uint8*)   &couleur.r, (Uint8*) &couleur.g, (Uint8*) &couleur.b);
 return couleur;
          
          
}


void  SDL_EcrireCouleurAux(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
     int bpp = surface->format->BytesPerPixel; 
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; 
  
  
    switch(bpp) { 
    case 1: 
        *p = pixel; 
        break; 
  
    case 2: 
        *(Uint16 *)p = pixel; 
        break; 
  
    case 3: 
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) { 
            p[0] = (pixel >> 16) & 0xff; 
            p[1] = (pixel >> 8) & 0xff; 
            p[2] = pixel & 0xff; 
        } else { 
            p[0] = pixel & 0xff; 
            p[1] = (pixel >> 8) & 0xff; 
            p[2] = (pixel >> 16) & 0xff; 
        } 
        break; 
  
    case 4: 
        *(Uint32 *)p = pixel; 
        break; 
    }    
}

void  SDL_EcrireCouleur(SDL_Surface* surface, int x, int y, Couleur couleur)
{
 SDL_EcrireCouleurAux(surface, x, y, 
    SDL_MapRGB(surface->format, couleur.r, couleur.g, couleur.b));
}

Couleur  couleurConvolutionNM(SDL_Surface* surface, int x, int y, Matrice* matrice)
{
 float r = 0;
 float g = 0;
 float b = 0;
 int i,j;
 Couleur coulGet;  /*aucun rapport avec la marque de dentifrice*/
 
 int m_lignes = matriceGetNbLignes(matrice);
 int m_colonnes = matriceGetNbColonnes(matrice);
 int m_milieu_lignes = (m_lignes-1) /2+1;
 int m_milieu_colonnes = (m_colonnes-1)/2+1;

 for (i=(m_milieu_lignes-m_lignes);i<=(m_lignes-m_milieu_lignes);i++)
  for (j=(m_milieu_colonnes-m_colonnes);j<=(m_lignes-m_milieu_colonnes);j++)
   {
    coulGet = SDL_LireCouleur(surface, x+i, y+j);
     r += ((float) coulGet.r) * matriceLire(matrice, i+m_milieu_lignes,j+m_milieu_colonnes);  
     g += ((float) coulGet.g) * matriceLire(matrice, i+m_milieu_lignes,j+m_milieu_colonnes);  
     b += ((float) coulGet.b) * matriceLire(matrice, i+m_milieu_lignes,j+m_milieu_colonnes);  
                   
   }           
 coulGet.r = (Uint8) mini(maxi(r,0.0f),255.0f);
 coulGet.g = (Uint8) mini(maxi(g,0.0f),255.0f);
 coulGet.b = (Uint8) mini(maxi(b,0.0f),255.0f);
 
 return coulGet;
}

SDL_Surface* convolveNM(SDL_Surface* origine, Matrice* matrice)
{
  SDL_Surface* destination;
  int i,j;            
  int hauteur = origine->h;
  int largeur = origine->w;
 
  Couleur cdest;
  if (origine==NULL)
   return NULL;
   
  destination = SDL_CreateRGBSurface(SDL_HWSURFACE, origine->w, origine->h, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);

  if (destination==NULL)
   return NULL;
 
  for(i=0; i<(largeur); i++)
   for (j=0; j<(hauteur); j++)
   {
    
       
    cdest = couleurConvolutionNM(origine, i, j,matrice);
    SDL_EcrireCouleur(destination, i, j, cdest);

  }
  
 return destination;          
}


///////////////// ALGO FILTRES /////////////


SDL_Surface* SDL_DetectionsBords(t_display display)
{
  Matrice m;
  SDL_Surface* surf;
  matriceInitialiser(&m, 3,3);

  matriceEcrire(&m, 1, 1, 0);
  matriceEcrire(&m, 1, 2, 1);
  matriceEcrire(&m, 1, 3, 0);
  matriceEcrire(&m, 2, 1, 1);
  matriceEcrire(&m, 2, 2, -4);
  matriceEcrire(&m, 2, 3, 1);
  matriceEcrire(&m, 3, 1, 0);
  matriceEcrire(&m, 3, 2, 1);
  matriceEcrire(&m, 3, 3, 0);

 
  surf = convolveNM(display.screen, &m);    
  matriceDetruire(&m);    

  SDL_BlitSurface(surf, NULL, display.screen, NULL);

  SDL_Flip(display.screen);

  return display.screen;        
}

SDL_Surface* SDL_Repoussage(t_display display)
{
  Matrice m;
  SDL_Surface* surf;
    
  matriceInitialiser(&m, 3,3); 
  
  matriceEcrire(&m, 1, 1, -2);
  matriceEcrire(&m, 1, 2, -1);
  matriceEcrire(&m, 1, 3, 0);
  matriceEcrire(&m, 2, 1, -1);
  matriceEcrire(&m, 2, 2, 1);
  matriceEcrire(&m, 2, 3, 1);
  matriceEcrire(&m, 3, 1, 0);
  matriceEcrire(&m, 3, 2, 1);
  matriceEcrire(&m, 3, 3, 2);


 
  surf = convolveNM(display.screen, &m);    
  matriceDetruire(&m);

  SDL_BlitSurface(surf, NULL, display.screen, NULL);

  SDL_Flip(display.screen);

  return surf;        
}

SDL_Surface* SDL_Flou(t_display display)
{
  Matrice m;
  SDL_Surface* surf;

  matriceInitialiser(&m, 3,3); 
  
  matriceEcrire(&m, 1, 1, 0.111111);
  matriceEcrire(&m, 1, 2, 0.111111);
  matriceEcrire(&m, 1, 3, 0.111111);
  matriceEcrire(&m, 2, 1, 0.111111);
  matriceEcrire(&m, 2, 2, 0.111111);
  matriceEcrire(&m, 2, 3, 0.111111);
  matriceEcrire(&m, 3, 1, 0.111111);
  matriceEcrire(&m, 3, 2, 0.111111);
  matriceEcrire(&m, 3, 3, 0.111111);  
 
  surf = convolveNM(display.screen, &m);    
  matriceDetruire(&m);

  SDL_BlitSurface(surf, NULL, display.screen, NULL);

  SDL_Flip(display.screen);
  
  return surf;    
}

SDL_Surface* SDL_Gaufrage(t_display display)
{
   Couleur ori;
   int i,j;
   SDL_Surface* dest;
   Matrice m;
  
  
   matriceInitialiser(&m, 3,3); 

   matriceEcrire(&m, 1, 1, -2);
   matriceEcrire(&m, 1, 2, -1);
   matriceEcrire(&m, 1, 3, 0);
   matriceEcrire(&m, 2, 1, -1);
   matriceEcrire(&m, 2, 2, 0);
   matriceEcrire(&m, 2, 3, 1);
   matriceEcrire(&m, 3, 1, 0);
   matriceEcrire(&m, 3, 2, 1);
   matriceEcrire(&m, 3, 3, 2);
   
   dest = convolveNM(display.screen, &m); 
 
 
   if (dest==NULL)
     return NULL;
      
   for(j=0;j< dest->h;j++)
    for(i=0;i< dest->w;i++)
    {
      ori = SDL_LireCouleur(dest, i, j);
      ori = couleurSomme(ori, couleurCreer((Uint8) 128,(Uint8) 128,(Uint8) 128));
      SDL_EcrireCouleur(dest, i,j, ori);            
    }       
 matriceDetruire(&m);

 SDL_BlitSurface(dest, NULL, display.screen, NULL);

 SDL_Flip(display.screen);


 return dest;
}

SDL_Surface* SDL_AugmenterContraste(t_display display)
{
  Matrice m;
  SDL_Surface* surf;
    
  matriceInitialiser(&m, 3,3); 
  
  matriceEcrire(&m, 1, 1, 0);
  matriceEcrire(&m, 1, 2, -1);
  matriceEcrire(&m, 1, 3, 0);
  matriceEcrire(&m, 2, 1, -1);
  matriceEcrire(&m, 2, 2, 5);
  matriceEcrire(&m, 2, 3, -1);
  matriceEcrire(&m, 3, 1, 0);
  matriceEcrire(&m, 3, 2, -1);
  matriceEcrire(&m, 3, 3, 0);
 
  surf = convolveNM(display.screen, &m);    
  matriceDetruire(&m); 

   SDL_BlitSurface(surf, NULL, display.screen, NULL);

   SDL_Flip(display.screen);

  
  return surf;     
}

SDL_Surface* SDL_FlouGaussien(t_display display)
{
  Matrice m;
  SDL_Surface* surf;
    
  matriceInitialiser(&m, 3,3); 
  
  matriceEcrire(&m, 1, 1, 1);
  matriceEcrire(&m, 1, 2, 2);
  matriceEcrire(&m, 1, 3, 1);
  matriceEcrire(&m, 2, 1, 2);
  matriceEcrire(&m, 2, 2, 4);
  matriceEcrire(&m, 2, 3, 2);
  matriceEcrire(&m, 3, 1, 1);
  matriceEcrire(&m, 3, 2, 2);
  matriceEcrire(&m, 3, 3, 1);
 
  surf = convolveNM(display.screen, &m);    
  matriceDetruire(&m); 

   SDL_BlitSurface(surf, NULL, display.screen, NULL);

   SDL_Flip(display.screen);

  
  return surf;     
}



