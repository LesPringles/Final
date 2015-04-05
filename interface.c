#include "interface.h"
#define UTF8(string) g_locale_to_utf8(string, -1, NULL, NULL, NULL)

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
GtkWidget* sdl_surface_to_GtkWidget(SDL_Surface* img);
void *run(void *arg);
void change_color(GtkWidget* widget, gpointer data);
void Function(GtkWidget* widget, gpointer data);
void Undo(GtkWidget* widget, gpointer data);
void Redo(GtkWidget* widget, gpointer data);
void Clear(GtkWidget* widget, gpointer data);
void Save(GtkWidget* widget, GtkWidget* file_selection);
void Open(GtkWidget* widget, GtkWidget* file_selection);

static GtkWidget *pRadioLabel;
static GtkWidget *pCheckLabel;
static GtkWidget *pTearoffLabel;

t_display display;

int main(int argc, char **argv)
{
	GtkWidget *pWindow;
	GtkWidget *pMenuBar;
	GtkWidget *pToolBar;
	GtkWidget *pVBox;
	GtkWidget *pVBox2;
	GtkWidget *pSdl;
	gchar *sTempLabel;	

	gtk_init(&argc, &argv);
  	
  SDL_Rect	pos;

  display.layers = NULL;
  display.action = PRINT_PIXEL;
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
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	    // Création de la GtkVBox 
	pVBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	    /**** Création du menu ****/
	pMenuBar = CreateMenu(pWindow);
		//Creation de la Toolbar
	pToolBar = Create_toolbar();
	
/* Creation de la deuxieme GtkVBox (pour les labels) */
  pVBox2 = gtk_vbox_new(FALSE, 0);
	/*	 
  pRadioLabel = gtk_label_new("Radio 1 est actif");
	gtk_box_pack_start(GTK_BOX(pVBox2), pRadioLabel, TRUE, TRUE, 0);
					  
	sTempLabel = g_locale_to_utf8("Check est décoché", -1, NULL, NULL, NULL);
	pCheckLabel = gtk_label_new(sTempLabel);
	g_free(sTempLabel);
	gtk_box_pack_start(GTK_BOX(pVBox2), pCheckLabel, TRUE, TRUE, 0);
	
	sTempLabel = g_locale_to_utf8("Menu attaché", -1, NULL, NULL, NULL);
	pTearoffLabel = gtk_label_new(sTempLabel);
	g_free(sTempLabel);
	gtk_box_pack_start(GTK_BOX(pVBox2), pTearoffLabel, TRUE, TRUE, 0);
	*/				

	pSdl = sdl_surface_to_GtkWidget(display.screen);
	gtk_box_pack_start(GTK_BOX(pVBox), pSdl, FALSE, FALSE, 0);

	  /* Ajout du menu a la fenêtre */
	gtk_box_pack_start(GTK_BOX(pVBox), pMenuBar, FALSE, FALSE, 0);
	  /* Ajout du toolbar a la fenetre */
	gtk_box_pack_start(GTK_BOX(pVBox), pToolBar, FALSE, FALSE, 0);
		/* Ajout des labels a la fenetre */
//	gtk_box_pack_start(GTK_BOX(pVBox), pVBox2, TRUE, TRUE, 0);
																												 
	gtk_widget_show_all(pWindow);																																	  
	
	pthread_t th0, th1;
	pthread_create(&th0, NULL, run, &display);
	pthread_create(&th1, NULL, run, NULL);
	pthread_join(th0, NULL);
	pthread_join(th1, NULL);
	
	/*sdl_loop(&display);
	gtk_main();*/


	SDL_Quit();
	 return EXIT_SUCCESS;
}

void OnRadio(GtkWidget* widget, gpointer data)
{
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
GtkWidget *pQuestion;
						 
	pQuestion = gtk_message_dialog_new(GTK_WINDOW(data),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_YES_NO,
		"Voulez vous vraiment\n"
		"quitter le programme?");
		
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
	GtkWidget *pAbout;
						 
	pAbout = gtk_message_dialog_new(GTK_WINDOW(data),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"Programme realise par Les Pringles\n"
			"Pour plus d'information:\n"
			"http://gtk.developpez.com");
								  
	gtk_dialog_run(GTK_DIALOG(pAbout));
											 
	gtk_widget_destroy(pAbout);
}

GtkWidget* Create_toolbar()
{
	GtkWidget *pToolbar = NULL;

	pToolbar = gtk_toolbar_new();

	/* Creation a partir de stock */
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_REFRESH,
		"Refresh",
		NULL,
		NULL,
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_CLEAR,
		"Clear",
		NULL,
		G_CALLBACK(Clear),
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
		NULL,
		NULL,
		-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_ZOOM_OUT,
		"Zoom Out",
		NULL,
		NULL,
		NULL,
		-1);

	gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
		GTK_STOCK_QUIT,
		"Fermer",
		NULL,
		G_CALLBACK(OnQuitter),
		NULL,
		-1);

		/* Modification de la taille des icones*/
//		gtk_toolbar_set_icon_size(GTK_TOOLBAR(pToolbar),GTK_ICON_SIZE_BUTTON);


		/* Affichage uniquement des icones*/
		gtk_toolbar_set_style(GTK_TOOLBAR(pToolbar),GTK_TOOLBAR_ICONS);
	
	return pToolbar;
}

GtkWidget* CreateMenu(GtkWidget* pWindow)
{
	GtkWidget *pMenu;
	GtkWidget *pMenuItem;
	GtkWidget *pMenuBar;
	GSList *pList;
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
		/* ETAPE 3*/   //Menu des fonction utilise pour draw ex: dessiner un carre ou un cercle
	pMenuItem = gtk_radio_menu_item_new_with_label(NULL, "Pixel");
	gtk_menu_shell_append(GTK_MENU_SHELL(pMenu), pMenuItem);
	pList2 = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(pMenuItem));
	g_signal_connect(G_OBJECT(pMenuItem), "activate", G_CALLBACK(Function), NULL);

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

	/** Quatrieme sous-menu **/
	  /* ETAPE 2 */
	pMenu = gtk_menu_new();
	  /* ETAPE 3 */
	pMenuItem = gtk_menu_item_new_with_label("A propos de...");
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
	selection = gtk_file_selection_new( g_locale_to_utf8( "Selectionnez un fichier", -1, NULL, NULL, NULL));
	gtk_widget_show(selection);

	gtk_window_set_modal(GTK_WINDOW(selection), TRUE);

	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(selection)->ok_button), "clicked", G_CALLBACK(Save), selection);

	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), selection);
}

void file_selection_load()
{
	GtkWidget *selection;
	selection = gtk_file_selection_new( g_locale_to_utf8( "Selectionnez un fichier", -1, NULL, NULL, NULL));
	gtk_widget_show(selection);

	gtk_window_set_modal(GTK_WINDOW(selection), TRUE);

	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(selection)->ok_button), "clicked", G_CALLBACK(Open), selection);

	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(selection)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), selection);

}

void recuperer_chemin(GtkWidget *bouton, GtkWidget *file_selection)
{
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
	const gchar* chemin;
	chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION (file_selection) );
	save(display.screen, chemin);
	gtk_widget_destroy(file_selection);
}

void Open(GtkWidget *bouton, GtkWidget *file_selection)
{
	const gchar* chemin;
	chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION (file_selection) );
	SDL_BlitSurface(SDL_LoadBMP(chemin), NULL, display.screen, NULL);
	gtk_widget_destroy(file_selection);
}

GtkWidget* sdl_surface_to_GtkWidget(SDL_Surface* img)
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
} 

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
const char *color;
color = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));


	if(strcmp(color, "RED") == 0)
		display.color_index = 0;
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
		display.color_index = 6;
	else if(strcmp(color, "WHITE") == 0)
		display.color_index = 7;
	else if(strcmp(color, "GREY") == 0)
		display.color_index = 8;
	else if(strcmp(color, "SILVER") == 0)
		display.color_index = 9;
}

void Function(GtkWidget* widget, gpointer data)
{
const char *fonction;
fonction = gtk_label_get_label(GTK_LABEL(GTK_BIN(widget)->child));

	if(strcmp(fonction, "Pixel")==0)
					display.action = 0;
	else if(strcmp(fonction, "Line")==0)
					display.action = 1;
	else if(strcmp(fonction, "Square")==0)
					display.action = 2;
	else if(strcmp(fonction, "Rectangle")==0)
					display.action = 3;
	else if(strcmp(fonction, "Circle")==0)
					display.action = 4;
	else if(strcmp(fonction, "Disc")==0)
					display.action = 5;
}

void Redo(GtkWidget* widget, gpointer data)
{
		redo(&display);
}

void Undo(GtkWidget* widget, gpointer data)
{
		undo(&display);
}

void Clear(GtkWidget* widget, gpointer data)
{
	new(display.screen);
}
