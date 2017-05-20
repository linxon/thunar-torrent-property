
static void torrent_page_set_title(TorrentPage* page, char* title) {
  g_message("Setting title: %s", title);
  gtk_label_set_text(GTK_LABEL(page->title), title);
}


static void torrent_page_set_leechers_value(TorrentPage* page, int leechers) {
  if (leechers == -1) {
    gtk_label_set_markup(GTK_LABEL(page->leechers), "<span foreground='red'><b>N/A</b></span>");
  } else {
    char *markup = g_strdup_printf ("<b><span foreground='red'>%i</span></b>", leechers);
    gtk_label_set_markup (GTK_LABEL (page->leechers), markup);
    g_free (markup);
  }
};


static void torrent_page_set_seeders_value(TorrentPage* page, int seeders) {
  if (seeders == -1) {
    gtk_label_set_markup(GTK_LABEL(page->seeders), "<span foreground='green'><b>N/A</b></span>");
  } else {
    char *markup = g_strdup_printf ("<b><span foreground='green'>%i</span></b>", seeders);
    gtk_label_set_markup (GTK_LABEL (page->seeders), markup);
    g_free (markup);
  }
};


static void torrent_page_update_torrent_status(gpointer args) {

  TorrentPage* page = args;
  gtk_widget_set_sensitive(page->refresh, FALSE);

  ThunarxFileInfo* info = torrent_page_get_file(page);
  TorrentInfo* torrent_info = torrent_info_from_thunarx_file_info(info);

  g_message("IN THREAD: Updating stats of %s", torrent_info->name);

  TorrentStatus* status = torrent_status_from_thunarx_file_info(THUNARX_FILE_INFO(info));
  torrent_page_set_seeders_value(TORRENT_PAGE(page), status->seeders);
  torrent_page_set_leechers_value(TORRENT_PAGE(page), status->leechers);
  g_message("Seeders: %i", status->seeders);
  g_message("Leechers: %i", status->leechers);

  gtk_widget_set_sensitive(page->refresh, TRUE);
}


static void torrent_page_on_refresh_clicked(GtkButton *button, gpointer callback_data) {
  g_thread_new (
    "refresh-thread", torrent_page_update_torrent_status, callback_data
  );
}

static GtkWidget* torrent_page_new_header(TorrentPage* page) {

  GtkWidget* header_hbox    = gtk_hbox_new(FALSE, 0);
  GtkWidget* header_icon    = gtk_image_new_from_icon_name("application-torrent", GTK_ICON_SIZE_DIALOG);
  GtkWidget* header_title   = gtk_label_new("Torrent name");
  GtkWidget* stats_vbox     = gtk_vbox_new(TRUE, 0);
  GtkWidget* seeders_hbox   = gtk_hbox_new(TRUE, 0);
  GtkWidget* leechers_hbox  = gtk_hbox_new(TRUE, 0);
  GtkWidget* seeders_label  = gtk_label_new("<b>Seeders:</b>");
  GtkWidget* leechers_label = gtk_label_new("<b>Leechers:</b>");
  GtkWidget* seeders_value  = gtk_label_new("<span foreground='green'><b>N/A</b></span>");
  GtkWidget* leechers_value = gtk_label_new("<span foreground='red'><b>N/A</b></span>");
  GtkWidget* refresh_button = gtk_button_new_from_stock(GTK_STOCK_REFRESH);

  g_signal_connect(G_OBJECT(refresh_button), "clicked", torrent_page_on_refresh_clicked, page);

  gtk_label_set_use_markup (GTK_LABEL(seeders_label), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(leechers_label), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(seeders_value), TRUE);
  gtk_label_set_use_markup (GTK_LABEL(leechers_value), TRUE);

  gtk_misc_set_alignment(GTK_MISC(seeders_label), 1.0f, 0.5f);
  gtk_misc_set_alignment(GTK_MISC(leechers_label), 1.0f, 0.5f);

  gtk_label_set_line_wrap (GTK_LABEL(header_title), TRUE);

  gtk_box_pack_start(GTK_BOX(header_hbox), header_icon, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(header_hbox), header_title, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(header_hbox), stats_vbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(header_hbox), refresh_button, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(stats_vbox), seeders_hbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(stats_vbox), leechers_hbox, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(seeders_hbox), seeders_label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(seeders_hbox), seeders_value, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(leechers_hbox), leechers_label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(leechers_hbox), leechers_value, FALSE, TRUE, 0);

  page->refresh = refresh_button;
  page->title = header_title;
  page->seeders = seeders_value;
  page->leechers = leechers_value;

  return header_hbox;
}
