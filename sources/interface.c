#include "../includes/interface.h"
#define UTF8(string) g_locale_to_utf8(string, -1, NULL, NULL, NULL)


void change_color(GtkWidget* widget, gpointer data);
int 	rotate_surface(SDL_Surface *surf, double angle);
int 	zoom_surface(SDL_Surface *surf, double zoom);

void fdessinfractale(t_display *display, int zoom, int Xrepereinit, int Yrepereinit, double paramCx, double paramCy);
void fractale(t_display *display, int x1, int y1, int x2, int y2, int level, int col);
void on_copier_button(GtkWidget *pButton, gpointer data);
void on_activate_entry(GtkWidget *pEntry, gpointer data);
void Text();


static GtkWidget *pRadioLabel;
static GtkWidget *pCheckLabel;
static GtkWidget *pTearoffLabel;

t_display display;
int value = 50;
char jojo[] = "50";

int main(int argc, char **argv)
{
	GtkWidget *pWindow;
	GtkWidget *pMenuBar;
//	GtkWidget *pImage;
	GtkWidget *pToolBar;
	GtkWidget *pVBox;

	gtk_init(&argc, &argv);

  SDL_Rect	pos;

  display.layers = NULL;
  display.action = PRINT_PIXEL_ROND;
  display.button = RELEASED;
  SDL_Init(SDL_INIT_VIDEO);

  display.screen = SDL_SetVideoMode(WINX, WINY, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

 	new(display.screen);
	SDL_WM_SetCaption("Apero", NULL);
 
 	pos.x = 0;
  pos.y = 0;
  add_layer(&display.layers, display.screen, &pos);

  display.color_index = 0;



	     // Création de la fenêtre
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(pWindow), "Apéro");
	gtk_window_set_default_size(GTK_WINDOW(pWindow), 0, 0);
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(quit), NULL);
	    // Création de la GtkVBox
	pVBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	    /**** Création du menu ****/
	pMenuBar = CreateMenu(pWindow);
		//Creation de la Toolbar
	pToolBar = Create_toolbar();


	  /* Ajout du menu a la fenêtre */
	gtk_box_pack_start(GTK_BOX(pVBox), pMenuBar, FALSE, FALSE, 0);
	  /* Ajout du toolbar a la fenetre */
	gtk_box_pack_start(GTK_BOX(pVBox), pToolBar, FALSE, FALSE, 0);
		/* Ajout d'un GTK_WIDGET*/
/*	pImage = gtk_image_new_from_file("./ressources/lena_couleur.bmp");
	gtk_box_pack_start(GTK_BOX(pVBox), pImage, FALSE, FALSE, 0);*/


	gtk_widget_show_all(pWindow);

	pthread_t th0, th1;
	pthread_create(&th0, NULL, run, &display);
	pthread_create(&th1, NULL, run, NULL);
	pthread_join(th0, NULL);
	pthread_join(th1, NULL);
	

	SDL_Quit();
	 return EXIT_SUCCESS;
}

void OnRadio(GtkWidget* widget, gpointer data)
{
(void)data;
const gchar *sRadioName;
gchar *sLabel;

  /* Récupérer le label du bouton radio active */
  sRadioName = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));

  sLabel = g_strdup_printf("%s est actif",sRadioName);
	gtk_label_set_label(GTK_LABEL(pRadioLabel), sLabel);
	g_free(sLabel);
}

void OnCheck(GtkWidget* widget, gpointer data)
{
 (void)data;
 gboolean bCoche;
 gchar *sLabel;
 gchar *sLabelUtf8;

  /* Savoir si le GtkCheckMenuItem est coche ou non */
  bCoche = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));

  if(bCoche)
  sLabel = g_strdup("Check est coché");
  else
  sLabel = g_strdup("Check est décoché");

  sLabelUtf8 = g_locale_to_utf8(sLabel, -1, NULL, NULL, NULL);

  gtk_label_set_label(GTK_LABEL(pCheckLabel), sLabelUtf8);
  g_free(sLabel);
	g_free(sLabelUtf8);
}

void OnTearoff(GtkWidget* widget, gpointer data)
{
	(void)widget;
	gboolean bDetache;
	gchar *sLabel;
	gchar *sLabelUtf8;

	/* Savoir si le menu est détaché ou non */
	bDetache = gtk_menu_get_tearoff_state(GTK_MENU(data));

	if(bDetache)
	sLabel = g_strdup("Menu détaché");
	else
	sLabel = g_strdup("Menu attaché");

	sLabelUtf8 = g_locale_to_utf8(sLabel, -1, NULL, NULL, NULL);

	gtk_label_set_label(GTK_LABEL(pTearoffLabel), sLabelUtf8);
	g_free(sLabel);
	g_free(sLabelUtf8);
}

void OnQuitter(GtkWidget* widget, gpointer data)
{
(void)widget;
GtkWidget *pQuestion;

	pQuestion = gtk_message_dialog_new(GTK_WINDOW(data),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_YES_NO,
		"Do you want to leave\n"
		"the program?");

		switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
		{
			case GTK_RESPONSE_YES:
					gtk_main_quit();
					SDL_Quit();
					break;
			case GTK_RESPONSE_NONE:
			case GTK_RESPONSE_NO:
					gtk_widget_destroy(pQuestion);
					break;
		}
}


void OnAbout(GtkWidget* widget, gpointer data)
{
	(void)widget;
	GtkWidget *pAbout;

	pAbout = gtk_message_dialog_new(GTK_WINDOW(data),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"Programme realise par Les Pringles\n"
			"Pour plus d'information:\n"
			"https://github.com/LesPringles/Final");

	gtk_dialog_run(GTK_DIALOG(pAbout));

	gtk_widget_destroy(pAbout);
}

GtkWidget* Create_toolbar()
{
	GtkWidget *pToolbar = NULL;

	pToolbar = gtk_toolbar_new();

	/* Creation a partir de stock */
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_CLEAR,
		"Clear",
		NULL,
		G_CALLBACK(Clear),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_SELECT_COLOR,
		"Palette",
		NULL,
		G_CALLBACK(Palette),
		NULL,
		-1);
gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_INDEX,
		"Pick Color",
		NULL,
		G_CALLBACK(PickColor),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_DIALOG_INFO,
		"Brightness",
		NULL,
		G_CALLBACK(Lumi),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_UNDO,
		"Undo",
		NULL,
		G_CALLBACK(Undo),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_REDO,
		"Redo",
		NULL,
		G_CALLBACK(Redo),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_ZOOM_IN,
		"Zoom In",
		NULL,
		G_CALLBACK(ZoomIN),
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_ZOOM_OUT,
		"Zoom Out",
		NULL,
		G_CALLBACK(ZoomOUT),
		NULL,
		-1);

	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_QUIT,
		"Close",
		NULL,
		G_CALLBACK(OnQuitter),
		NULL,
		-1);


		/* Affichage uniquement des icones*/
		gtk_toolbar_set_style(GTK_TOOLBAR(pToolbar),GTK_TOOLBAR_ICONS);

	return pToolbar;
}

GtkWidget* CreateMenu(GtkWidget* pWindow)
{
	GtkWidget *pMenu;
	GtkWidget *submenu;
	GtkWidget *pMenuItem;
	GtkWidget *pMenuBar;
	GSList *pList2;
	GSList *pListC;
	char *color[] = {
					"BLACK",
					"BLUE",
					"GREEN",
					"YELLOW",
					"PINK",
					"PURPLE",
					"RED",
					"WHITE",
					"GREY",
					"SILVER"
	};

	/**** Création du menu ****/

	    /* ETAPE 1 */
	pMenuBar = gtk_menu_bar_new();

	    /** Premier sous-menu **/
	    /* ETAPE 2 */
	pMenu = gtk_menu_new();
			/* ETAPE 3 */

		   /* GtkImageMenuItem */
	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Clear), NULL);
  //fonction new sdl screen

	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(file_selection_load), NULL);
	//ouvrir un fichier

	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
  g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(file_selection_save), NULL);
	//fonction pour save image a l'endroit choisi

	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(quit), NULL);
	//fermer l'image/widget sdl

		/* GtkSeparatorItem */
  pMenuItem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnQuitter), (GtkWidget*) pWindow);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);

    /* ETAPE 4 */
  pMenuItem = gtk_menu_item_new_with_label("File");
	  /* ETAPE 5 */
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
	  /* ETAPE 6 */
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);

	/*  Deuxieme sous-menu*/
		/* ETAPE 2*/
	pMenu = gtk_menu_new();
		/* ETAPE 3*/   

	 /* SubMenu*/
	 	/*ETAPE 2*/
	submenu = gtk_menu_new();
		/*ETAPE 3*/
	pMenuItem = gtk_radio_menu_item_new_with_label(NULL, "Round");
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Square");
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	/*ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Pixel");
	/*ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), submenu);
	/*ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);


	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Line");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Square");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Rectangle");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Circle");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Disc");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Ellipse");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Star");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "Eraser");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

	pMenuItem = gtk_radio_menu_item_new_with_label(pList2, "FillPot");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

		/* ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Tools");
		/* ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
		/* ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);

	/** Troisieme sous-menu **/
		/* ETAPE 2*/
	pMenu = gtk_menu_new();
		/* ETAPE 3*/
	pMenuItem = gtk_radio_menu_item_new_with_label(NULL, "BLACK");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pListC = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(change_color), NULL);

	for(int i = 1; i < 9 ; i++)
	{
	pMenuItem = gtk_radio_menu_item_new_with_label(pListC, color[i]);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pListC = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(change_color), NULL);
	}
		/* ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Color");
		/* ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
		/* ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);


	/** Quatrieme sous-menu**/
		/* ETAPE 2*/
	pMenu = gtk_menu_new();

		/* ETAPE 3*/
	pMenuItem = gtk_menu_item_new_with_label( "+90");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Rotate), NULL);

	pMenuItem = gtk_menu_item_new_with_label("180");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Rotate), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "-90");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Rotate), NULL);


				 /* SubMenu*/
			 	/*ETAPE 2*/
			submenu = gtk_menu_new();
				/*ETAPE 3*/
			pMenuItem = gtk_menu_item_new_with_label("Horizontal");
			gtk_menu_shell_append(GTK_MENU_SHELL(submenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Rotate), NULL);

			pMenuItem = gtk_menu_item_new_with_label("Vertical");
			gtk_menu_shell_append(GTK_MENU_SHELL(submenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Rotate), NULL);

			/*ETAPE 4*/
			pMenuItem = gtk_menu_item_new_with_label("Symmetry");
			/*ETAPE 5*/
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), submenu);
			/*ETAPE 6*/
			gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);



		/* ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Rotate");
		/* ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
		/* ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);


	/** Cinquieme sous-menu**/
		/* ETAPE 2*/
	pMenu = gtk_menu_new();
		/* ETAPE 3*/
	pMenuItem = gtk_menu_item_new_with_label( "FilterB/W");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterBW), NULL);

	pMenuItem = gtk_menu_item_new_with_label("FilterGreen");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterG), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterNegative");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterInv), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterBlue");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterBlue), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterGreen");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterGreen), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterRed");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterRed), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterBright+");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterLumPlus), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "FilterBright-");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FilterLumMoins), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Detec");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Detec), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Bumpmaps");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Repoussage), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Blur");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Flou), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Embossing");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Gaufrage), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Increase Contrast");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(AugContraste), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Gaussian Blur");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(FlouGaussien), NULL);



		/* ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Filter");
		/* ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
		/* ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);

	/** Sixieme sous-menu**/
		/* ETAPE 2*/
	pMenu = gtk_menu_new();
		/* ETAPE 3*/
	pMenuItem = gtk_menu_item_new_with_label( "Mandelbrot's Fractal");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Fractales), NULL);

	pMenuItem = gtk_menu_item_new_with_label("Julia's Fractal");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Fractales), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Julia's Fractal II");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Fractales), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Julia's Fractal : Spiral Out");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Fractales), NULL);

	pMenuItem = gtk_menu_item_new_with_label( "Koch Snowflake");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Fractales), NULL);




		/* ETAPE 4*/
	pMenuItem = gtk_menu_item_new_with_label("Fractals");
		/* ETAPE 5*/
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
		/* ETAPE 6*/
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);


	/** Septieme sous-menu **/
	  /* ETAPE 2 */
	pMenu = gtk_menu_new();
 /* ETAPE 3 */
	pMenuItem = gtk_menu_item_new_with_label("About...");
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(OnAbout), (GtkWidget*) pWindow);
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	  /* ETAPE 4 */
	pMenuItem = gtk_menu_item_new_with_label("?");
	  /* ETAPE 5 */
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pMenu);
	  /* ETAPE 6 */
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenuBar), pMenuItem);

	return pMenuBar;
}

void file_selection_save()
{
	GtkWidget *selection;
	selection = gtk_file_selection_new( g_locale_to_utf8( "Select file", -1, NULL, NULL, NULL));
	gtk_widget_show(selection);

	gtk_window_set_modal(GTK_WINDOW(selection), TRUE);

	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(selection)->ok_button), "clicked", G_CALLBACK(Save), selection);

	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), selection);
}

void file_selection_load()
{
	GtkWidget *selection;
	selection = gtk_file_selection_new( g_locale_to_utf8( "Select file", -1, NULL, NULL, NULL));
	gtk_widget_show(selection);

	gtk_window_set_modal(GTK_WINDOW(selection), TRUE);

	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(selection)->ok_button), "clicked", G_CALLBACK(Open), selection);

	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), selection);

}

void recuperer_chemin(GtkWidget *bouton, GtkWidget *file_selection)
{
	(void)bouton;
    const gchar* chemin;
    GtkWidget *dialog;
    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION (file_selection) );

    dialog = gtk_message_dialog_new(GTK_WINDOW(file_selection),
    GTK_DIALOG_MODAL,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Vous avez choisi :\n%s", chemin);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(file_selection);
}

void Save(GtkWidget *bouton, GtkWidget *file_selection)
{
	(void)bouton;
	const gchar* chemin;
	chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION (file_selection) );
	save(display.screen, chemin);
	gtk_widget_destroy(file_selection);
}

void Open(GtkWidget *bouton, GtkWidget *file_selection)
{
	SDL_Rect		pos_0;

  	pos_0.x = 0;
  	pos_0.y = 0;


	(void)bouton;
	const gchar* chemin;
	chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION (file_selection) );
	new(display.screen);
	SDL_BlitSurface(SDL_LoadBMP(chemin), NULL, display.screen, NULL);
	SDL_Flip(display.screen);
	gtk_widget_destroy(file_selection);

	add_layer(&display.layers, display.screen, &pos_0);
}

/*GtkWidget* sdl_surface_to_GtkWidget(SDL_Surface* img)
{
    SDL_LockSurface(img);
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_data
        (
            NULL,
            GDK_COLORSPACE_RGB,
            TRUE,
            8,
            img->w,
            img->h,
            img->pitch,
            NULL,
            NULL
        );
    SDL_UnlockSurface(img);
    return gtk_image_new_from_pixbuf(pixbuf);
} */

void *run(void *arg){
	if(arg != NULL)
			{
			sdl_loop(arg);
			}
	else
			{
			gtk_main();
			}
	pthread_exit(NULL);
}

void change_color(GtkWidget* widget, gpointer data)
{
  (void)data;
  const char *color;
  color = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));


  if(strcmp(color, "RED") == 0)
    display.color_index = 6;
  else if(strcmp(color, "BLUE") == 0)
    display.color_index = 1;
  else if(strcmp(color, "GREEN") == 0)
    display.color_index = 2;
  else if(strcmp(color, "YELLOW") == 0)
    display.color_index = 3;
  else if(strcmp(color, "PINK") == 0)
    display.color_index = 4;
  else if(strcmp(color, "PURPLE")==0)
    display.color_index = 5;
  else if(strcmp(color, "BLACK") == 0)
    display.color_index = 0;
  else if(strcmp(color, "WHITE") == 0)
    display.color_index = 7;
  else if(strcmp(color, "GREY") == 0)
    display.color_index = 8;
  else if(strcmp(color, "SILVER") == 0)
    display.color_index = 9;
  display.current_color = colors[display.color_index];
}

void Function(GtkWidget* widget, gpointer data)
{
(void)data;
const char *fonction;
fonction = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));

	if(strcmp(fonction, "Squre")==0)
					display.action = 0;	
	else if(strcmp(fonction, "Round")==0)
					display.action = 1;
	else if(strcmp(fonction, "Line")==0)
					display.action = 2;
	else if(strcmp(fonction, "Square")==0)
					display.action = 3;
	else if(strcmp(fonction, "Rectangle")==0)
					display.action = 4;
	else if(strcmp(fonction, "Circle")==0)
					display.action = 5;
	else if(strcmp(fonction, "Disc")==0)
					display.action = 6;
	else if(strcmp(fonction, "Ellipse")==0)
					display.action = 7;
	else if(strcmp(fonction, "Star")==0)
					display.action = 8;
	else if(strcmp(fonction, "Eraser")==0)
					display.action = 9;
	else if(strcmp(fonction, "FillPot")==0)
					display.action = 10;

}

void FilterBW()
{
		filtre_noir_blanc(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterG()
{
		filtre_gris(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterInv()
{
		filtre_inverser(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterBlue()
{
		filtre_bleu(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterGreen()
{
		filtre_vert(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterRed()
{
		filtre_rouge(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterLumPlus()
{
		filtre_lum_plus(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FilterLumMoins()
{
		filtre_lum_moins(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void Detec()
{
		SDL_DetectionsBords(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void Repoussage()
{
		SDL_Repoussage(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void Flou()
{
		SDL_Flou(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void Gaufrage()
{
		SDL_Gaufrage(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void AugContraste()
{
		SDL_AugmenterContraste(display);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}

void FlouGaussien()
{
		SDL_FlouGaussien(display, 0.05, 15);

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;

		add_layer(&display.layers, display.screen, &pos_0);

}




void Lumi()
{
	 GtkWidget* pWindow;
   GtkWidget *pMainVBox;
   GtkWidget *pFrame;
   GtkWidget *pColorBox;
   GtkWidget *pLabel;
   GtkWidget *pScrollbar;
   GtkObject *Adjust;

		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;


   pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(pWindow), "Ricard!");
   gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 50);
   gtk_container_set_border_width(GTK_CONTAINER(pWindow), 4);
 
   pMainVBox = gtk_vbox_new(TRUE, 0);
   gtk_container_add(GTK_CONTAINER(pWindow), pMainVBox);
 
   pFrame = gtk_frame_new("Brightness");
   gtk_box_pack_start(GTK_BOX(pMainVBox), pFrame, FALSE, FALSE, 0);
   pColorBox = gtk_vbox_new(TRUE, 0);
   gtk_container_add(GTK_CONTAINER(pFrame), pColorBox);
	 
   /* Label d'affichage de valeur R*/
   pLabel = gtk_label_new(jojo);
   gtk_box_pack_start(GTK_BOX(pColorBox), pLabel, FALSE, FALSE, 0);
   /* Création d un GtkAdjustment */
   Adjust = gtk_adjustment_new(value, 0, 100, 1, 10, 1);
   /* Creation d une scrollbar horizontale*/
   pScrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(Adjust));
   gtk_box_pack_start(GTK_BOX(pColorBox), pScrollbar, TRUE, TRUE, 0);
   /* Connexion du signal pour modification de l'affichage */
   g_signal_connect(G_OBJECT(pScrollbar), "value-changed",
   		G_CALLBACK(OnScrollbarChange), (GtkWidget*)pLabel);
 
   gtk_widget_show_all(pWindow);
 
   g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
   gtk_main();

	if(value > 50)
	{
	filtre_bright_p(display, value);
	add_layer(&display.layers, display.screen, &pos_0);
	}
	else if(value < 50)
	{
	filtre_bright_m(display, value);
	add_layer(&display.layers, display.screen, &pos_0);
	}

}

void OnScrollbarChange(GtkWidget *pWidget, gpointer data)
{
   gchar* sLabel;
   gint iValue;
 /*		SDL_Rect		pos_0;

    	pos_0.x = 0;
    	pos_0.y = 0;*/

   /* Récupération de la valeur de la scrollbar */
   iValue = gtk_range_get_value(GTK_RANGE(pWidget));
	 value = gtk_range_get_value(GTK_RANGE(pWidget));
	 sprintf(jojo, "%d", value);
   /* Création du nouveau label */
   sLabel = g_strdup_printf("%d", iValue);
   /* Modification du label */
   gtk_label_set_text(GTK_LABEL(data), sLabel);
	 /*Update*/
/*	 	if(value > 50)
	{
	filtre_bright_p(display, value);
	add_layer(&display.layers, display.screen, &pos_0);
	}
	else if(value < 50)
	{
	filtre_bright_m(display, value);
	add_layer(&display.layers, display.screen, &pos_0);
	}*/
   /* Liberation memoire */
   g_free(sLabel);
}

void Redo()
{
		redo(&display);
}

void Undo()
{
		undo(&display);
}

void Clear()
{
		new(display.screen);
		value = 50;
		
}

void quit()
{
	SDL_Quit();
	gtk_main_quit();
}

void Palette()
{
 getColorFromPalette(&display, 1);
}

void PickColor()
{
	SDL_Rect		pos_0;

    pos_0.x = 0;
    pos_0.y = 0;

	add_layer(&display.layers, display.screen, &pos_0);
  getColorFromPalette(&display, 0);
}

void Rotate(GtkWidget* widget, gpointer data)
{
	(void)data;
	const char *fonction;
	fonction = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));

	if(strcmp(fonction, "+90")==0)
			{rotate_surface(display.screen, -90.0);}
	else if(strcmp(fonction, "180")==0)
			{/*
			rotate_surface(display.screen, -90.0);
			rotate_surface(display.screen, -90.0);*/
			rotate_surface(display.screen, 180.0);
			}
	else if(strcmp(fonction, "-90")==0)
			{/*
			rotate_surface(display.screen, -90.0);
			rotate_surface(display.screen, -90.0);
			rotate_surface(display.screen, -90.0);*/
			rotate_surface(display.screen, 90.0);
			}
	else if(strcmp(fonction, "Horizontal")==0)
			{
				rotation_horizontale(display);
				SDL_Rect		pos_0;

  		  pos_0.x = 0;
  			 pos_0.y = 0;

				add_layer(&display.layers, display.screen, &pos_0);
 				 getColorFromPalette(&display, 0);
			}
	else if(strcmp(fonction, "Vertical")==0)
			{
				rotation_verticale(display);
			}


	
			
}

#define LEVEL 5 

void Fractales(GtkWidget* widget, gpointer data)
{
	(void)data;
	const char *ffractale;
	ffractale = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));


	if(strcmp(ffractale, "Mandelbrot's Fractal")==0)
			{
				fdessinfractale(&display, 1, 0, 0, 0, 0);	
			}
	else if(strcmp(ffractale, "Julia's Fractal")==0)
			{
				fdessinfractale(&display, 1, 0, 0, -0.7927, 0.1609);	
			}
	else if(strcmp(ffractale, "Julia's Fractal II")==0)
			{
				fdessinfractale(&display, 1, 0, 0, 0.32, 0.043);	
			}
	else if(strcmp(ffractale, "Julia's Fractal : Spiral Out")==0)
			{
				fdessinfractale(&display, 1, 0, 0, -0.0986, -0.65186);	
			}
	else if(strcmp(ffractale, "Koch Snowflake")==0)
			{
				fractale(&display, 320, 60, 520, 340, LEVEL, 0);
    			fractale(&display, 520, 340, 120, 340, LEVEL, 0);
    			fractale(&display, 120, 340, 320, 60, LEVEL, 0);
				SDL_Flip(display.screen);
			}

}

void ZoomIN()
{
	SDL_Rect		pos_0;

    pos_0.x = 0;
    pos_0.y = 0;

	zoom_surface(display.screen, 1.1);

	add_layer(&display.layers, display.screen, &pos_0);

}

void ZoomOUT()
{
	SDL_Rect		pos_0;

    pos_0.x = 0;
    pos_0.y = 0;

	zoom_surface(display.screen, 0.9);

	add_layer(&display.layers, display.screen, &pos_0);

}
