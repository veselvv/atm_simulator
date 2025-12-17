#include "../core/database.h"
#include "../core/card.h"
#include "../core/file_io.h"
#include "../core/operation.h"
#include <sys/stat.h>
#include <string.h>
#include <gtk/gtk.h>



typedef enum {
    LOGIN_MODE,    // по логину
    PHONE_MODE,    // по телефону  
    CARD_MODE      // по номеру карты
} LoginMode;

typedef enum{
    TRANSFER_CARD_MODE,
    TRANSFER_PHONE_MODE
} TransferMode;



typedef struct{
    GtkWindow *main_window;
    GtkWidget *Entry_pass;
    GtkWidget *Entry_login;
    Database *db;
    GtkWidget *main_stack;     // stack
    GtkWidget *balance_label;
    GtkWidget *login_page;    // страница входа
    GtkWidget *cabinet_page;
    GtkWidget *last_button;
    GtkWidget *main_balance_label;
    Card *current_card;
    GtkWidget *button;
    GtkWidget *label_user;
    GtkWidget *choice_button_phone;
    GtkWidget *choice_button_card;
    GtkWidget *choice_button_login;
    GtkWidget *main_card_label;
    LoginMode current_mode;

    GtkWidget *animation_page;
    
    GtkWidget *transfer_button;
    GtkWidget *give_money_button;
    GtkWidget *main_gender_label;

    GtkDialog *give_money_dialog;
    GtkDialog *translate_dialog;
    GtkDialog *take_money_dialog;
    GtkWidget *take_money_button;
    GtkWidget *give_money_button_dialog;
    GtkWidget *give_money_entry_dialog;
    GtkWidget *person_image;
    GtkWidget *take_money_entry_dialog;
    GtkWidget *take_money_button_dialog;
    GtkWidget *translate_entry_card_dialog;
    GtkWidget *translate_entry_money_dialog;
    GtkWidget *translate_button_dialog;

    TransferMode current_transfer_mode;
    GtkWidget *transfer_button_change;

    GtkWidget *history_button;
    char* file_history_name;
    const char *data_path;



    GtkWidget *history_labels_container;
}AppData;


// static void add_operrations_labels(gpointer userdata){
//     AppData *data = (AppData*)userdata;
    




// }






static void show_alert(GtkWindow *parent, const char *message) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new(message);
    gtk_alert_dialog_show(dialog, parent);
    g_object_unref(dialog);
}

static void on_text_changed(GtkWidget *Entry, gpointer userdata){
    AppData *data = (AppData*)userdata;
    const char *text = gtk_editable_get_text(GTK_EDITABLE(data->Entry_login));
    switch(data->current_mode){
        case CARD_MODE:
            if(strlen(text)<16){
                gtk_widget_add_css_class(GTK_WIDGET(data->Entry_login), "Entry_login_validation");
            }
            else{
                gtk_widget_remove_css_class(GTK_WIDGET(data->Entry_login), "Entry_login_validation");
            }
    }
}



static void open_log_file(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    char command[256];
    snprintf(command, sizeof(command), "xdg-open \"%s\"", data->file_history_name);
    system(command);
}

static gboolean load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gboolean success = FALSE;
    
    // Загружаем CSS файл
    gtk_css_provider_load_from_path(provider, "style.css");
        // Применяем CSS ко всему приложению
    gtk_style_context_add_provider_for_display(
            gdk_display_get_default(),
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
        success = TRUE;
        g_print("CSS loaded successfully\n");
    
    g_object_unref(provider);
    return success;
}


double safe_string_to_double(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return -1.0;
    }
    
    char buffer[100];
    strncpy(buffer, str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    // Заменяем запятые на точки
    for (char *p = buffer; *p; p++) {
        if (*p == ',') *p = '.';
    }
    
    // Убираем пробелы и символы новой строки
    char clean[100] = {0};
    int j = 0;
    for (int i = 0; buffer[i] != '\0' && j < sizeof(clean) - 1; i++) {
        char c = buffer[i];
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            clean[j++] = c;
        }
    }
    
    if (clean[0] == '\0') {
        return -1.0;
    }
    
    char *endptr;
    double value = strtod(clean, &endptr);
    
    // Проверяем что обработана вся строка
    if (endptr == clean || *endptr != '\0') {
        return -1.0;
    }
    
    return value;
}
static void show_dialog_transfer(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    gtk_window_present(GTK_WINDOW(data->translate_dialog));

}

static void show_dialog_take_money(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    gtk_window_present(GTK_WINDOW(data->take_money_dialog));

}


static void show_give_money_dialog(GtkWidget *btn,gpointer userdata){
    AppData *data = (AppData*)userdata;
    gtk_window_present(GTK_WINDOW(data->give_money_dialog));
}

static gboolean switch_to_cabinet(gpointer userdata) {
    AppData *data = (AppData*)userdata;
    gtk_stack_set_visible_child(GTK_STACK(data->main_stack), data->cabinet_page);
    return G_SOURCE_REMOVE;
}

static void start_fade_transition(gpointer userdata){
    AppData *data=(AppData*)userdata;
    GtkStack *stack = GTK_STACK(data->main_stack);
    gtk_stack_set_visible_child(stack, data->animation_page);
    g_timeout_add(200,switch_to_cabinet,data);


}

static gboolean on_dialog_close_request(GtkWindow *dialog) {
    // Скрываем диалог вместо уничтожения
    gtk_widget_set_visible(GTK_WIDGET(dialog), FALSE);
    return TRUE; // Предотвращаем стандартную обработку закрытия
}

static void balance_up_dialog_button(GtkWidget *btn, gpointer userdata){
    AppData *data  = (AppData*)userdata;
    const char* text = gtk_editable_get_text(GTK_EDITABLE(data->give_money_entry_dialog));
    double value = safe_string_to_double(text);

    if (text == NULL || text[0] == '\0') {
        return;
    }

    if(value!=-1){
        if(replenish_balance(data->current_card, value)){
            if (save_db_to_jsoon(data->db, data->data_path)) {
                g_print("Данные успешно сохранены\n");
            } else {
                g_print("Ошибка сохранения данных!\n");
            }
            double balance = data->current_card->balance;
            char balance_str[50];
            snprintf(balance_str, sizeof(balance_str), "%.2fР", balance);
            gtk_label_set_text(GTK_LABEL(data->main_balance_label), balance_str);
            gtk_editable_set_text(GTK_EDITABLE(data->give_money_entry_dialog),"");
            on_dialog_close_request(GTK_WINDOW(data->give_money_dialog));
            show_alert(data->main_window, "Операция выполнена успешно");
        }
        else{
            gtk_editable_set_text(GTK_EDITABLE(data->give_money_entry_dialog),"");
            on_dialog_close_request(GTK_WINDOW(data->give_money_dialog));
            show_alert(data->main_window, "Неудалось выполнить операцию");
        }
        

    }
    else{
        gtk_editable_set_text(GTK_EDITABLE(data->give_money_entry_dialog),"");
        show_alert(data->main_window, "Неудалось выполнить операцию");
        printf("Не удалось преобразовать строку в формат double\n");
    }

}
static void balance_low_dialog_button(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    const char *text = gtk_editable_get_text(GTK_EDITABLE(data->take_money_entry_dialog));
    double value = safe_string_to_double(text);
    if (text == NULL || text[0] == '\0') {
        return;
    }
    if(value!=-1){
        if(withdraw_balance(data->current_card, value)){
            if (save_db_to_jsoon(data->db, data->data_path)) {
                g_print("Данные успешно сохранены\n");
            } else {
                g_print("Ошибка сохранения данных!\n");
            }
            double balance = data->current_card->balance;
            char balance_str[50];
            snprintf(balance_str, sizeof(balance_str), "%.2fР", balance);
            gtk_label_set_text(GTK_LABEL(data->main_balance_label), balance_str);
            gtk_editable_set_text(GTK_EDITABLE(data->take_money_entry_dialog),"");
            on_dialog_close_request(GTK_WINDOW(data->take_money_dialog));
            show_alert(data->main_window, "Операция выполнена успешно");
        }
        else{
            gtk_editable_set_text(GTK_EDITABLE(data->take_money_entry_dialog),"");
            on_dialog_close_request(GTK_WINDOW(data->take_money_dialog));
            show_alert(data->main_window, "Неудалось выполнить операцию");

        }

    }
    else{
        show_alert(data->main_window, "Введены невалидные данные");
        gtk_editable_set_text(GTK_EDITABLE(data->take_money_entry_dialog),"");
        printf("Не удалось преобразовать строку в формат double\n");
    }
    


}
static void transfer_button_change_press(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    switch (data->current_transfer_mode){
    case TRANSFER_CARD_MODE:
        data->current_transfer_mode = TRANSFER_PHONE_MODE;
        gtk_entry_set_placeholder_text(GTK_ENTRY(data->translate_entry_card_dialog), "phone number");
        gtk_button_set_label(GTK_BUTTON(btn), "Перевод по номеру карты");
        break;
    
    case TRANSFER_PHONE_MODE:
        data->current_transfer_mode=TRANSFER_CARD_MODE;
        gtk_entry_set_placeholder_text(GTK_ENTRY(data->translate_entry_card_dialog), "card number");
        gtk_button_set_label(GTK_BUTTON(btn), "Перевод по номеру телефона");

        break;
    default:
        g_print("Не удалось сменить мод\n");
    }
    
}

static void balance_translate_dialog_button(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    const char *text_entry_card = gtk_editable_get_text(GTK_EDITABLE(data->translate_entry_card_dialog));
    const char *text_entry_money = gtk_editable_get_text(GTK_EDITABLE(data->translate_entry_money_dialog));
    double value = safe_string_to_double(text_entry_money);
    Card *card1 = data->current_card;
    Card *card2 = NULL;
    if (text_entry_card == NULL || text_entry_card[0] == '\0'||text_entry_money==NULL||text_entry_money[0]=='\0') {
        return;
    }
    if(value!=-1){
        switch (data->current_transfer_mode){
            case  TRANSFER_CARD_MODE:
            card2 = find_card_by_CardNumber(data->db, text_entry_card);
            if(transform_balance_by_CardNumber(card1,card2,value)){
                double balance1 = card1->balance;
                char balance_str1[50];
                snprintf(balance_str1, sizeof(balance_str1), "%.2fР", balance1);
                gtk_label_set_text(GTK_LABEL(data->main_balance_label), balance_str1);
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_card_dialog),"");
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_money_dialog),"");
                on_dialog_close_request(GTK_WINDOW(data->translate_dialog));
                show_alert(data->main_window, "Операция выполнена успешно");

            }
            else{
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_card_dialog),"");
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_money_dialog),"");
                on_dialog_close_request(GTK_WINDOW(data->translate_dialog));
                show_alert(data->main_window, "Неудалось выполнить операцию");
            }
            break;
        
        case TRANSFER_PHONE_MODE:
            card2 = find_card_by_PhoneNumber(data->db,text_entry_card);
            if(transform_balance_by_PhoneNumber(card1,card2,value)){
                double balance1 = card1->balance;
                char balance_str1[50];
                snprintf(balance_str1, sizeof(balance_str1), "%.2fР", balance1);
                gtk_label_set_text(GTK_LABEL(data->main_balance_label), balance_str1);
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_card_dialog),"");
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_money_dialog),"");
                on_dialog_close_request(GTK_WINDOW(data->translate_dialog));
                show_alert(data->main_window, "Операция выполнена успешно");
                
            }
            else{
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_card_dialog),"");
                gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_money_dialog),"");
                on_dialog_close_request(GTK_WINDOW(data->translate_dialog));
                show_alert(data->main_window, "Неудалось выполнить операцию");
            }
            break;
        default:
            break;
        }
        if (save_db_to_jsoon(data->db, data->data_path)) {
            g_print("Данные успешно сохранены\n");
        } else {
            g_print("Ошибка сохранения данных!\n");
        }
    }
    else{
        show_alert(data->main_window, "Введены невалидные данные");
        gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_money_dialog),"");
        gtk_editable_set_text(GTK_EDITABLE(data->translate_entry_card_dialog),"");
        printf("Не удалось преобразовать строку в формат double\n");

    }
    

}


static void close_and_save(GtkWidget *window, gpointer userdata){
    AppData *data=(AppData*)userdata;
    if (save_db_to_jsoon(data->db, data->data_path)) {
        g_print("Данные успешно сохранены\n");
    } else {
        g_print("Ошибка сохранения данных!\n");
    }
    g_application_quit(G_APPLICATION(gtk_window_get_application(GTK_WINDOW(window))));
}

static void click_last(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData *)userdata;


    gtk_editable_set_text(GTK_EDITABLE(data->Entry_login), "");
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");
    data->current_card=NULL;
    free(data->file_history_name);
    data->file_history_name=NULL;
    gtk_stack_set_visible_child(GTK_STACK(data->main_stack), data->login_page);


}

static void button_change_enter_phone(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData *)userdata;
    data->current_mode=PHONE_MODE;
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_login), "");
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");
    gtk_entry_set_placeholder_text(GTK_ENTRY(data->Entry_login), "phone number");
    gtk_entry_set_max_length(GTK_ENTRY(data->Entry_login),30);
}


static void button_change_enter_card(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    data->current_mode=CARD_MODE;
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_login), "");
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");
    gtk_entry_set_placeholder_text(GTK_ENTRY(data->Entry_login), "card number");
    gtk_entry_set_max_length(GTK_ENTRY(data->Entry_login),16);
}

static void button_change_enter_login(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData*)userdata;
    data->current_mode=LOGIN_MODE;
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_login), "");
    gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");
    gtk_entry_set_placeholder_text(GTK_ENTRY(data->Entry_login), "login");
    gtk_entry_set_max_length(GTK_ENTRY(data->Entry_login),30);
}





static void button_clicked(GtkWidget *btn, gpointer userdata){
    AppData *data = (AppData *)userdata;
    const char *login = gtk_editable_get_text(GTK_EDITABLE(data->Entry_login));
    const char *pass = gtk_editable_get_text(GTK_EDITABLE(data->Entry_pass));
    Card* card = NULL;
    switch (data->current_mode){
        case PHONE_MODE:
            card=find_card_by_PhoneNumber(data->db, login);
            break;
        case CARD_MODE:
            card=find_card_by_CardNumber(data->db, login);
            break;;
        case LOGIN_MODE:
            card=find_card_by_Login(data->db,login);
            break;    
        default:
            break;
    }
    if(card!=NULL){
        if(card->is_blocked==1){
            printf("Нельзя войти в личный кабинет, ваша карта заблокирована\n");
            show_alert(data->main_window,"Нельзя войти в личный кабинет, ваша карта заблокирована");
            return;
        }
        if((strcmp(card->pin, pass) == 0)&&card->pin_atempts>0&&card->is_blocked!=1){
            print_card_info(card);
            card->pin_atempts=3;
            data->current_card=card;
            double balance = data->current_card->balance;
            char balance_str[50];
            if(strcmp(card->gender,"F")==0){
                gtk_image_set_from_file(GTK_IMAGE(data->person_image),"pic/women.png");
                gtk_label_set_text(GTK_LABEL(data->main_gender_label),"Женский");

            }else if(strcmp(card->gender,"f")==0){
                gtk_image_set_from_file(GTK_IMAGE(data->person_image),"pic/women.png");
                gtk_label_set_text(GTK_LABEL(data->main_gender_label),"Женский");
            }else{
                gtk_image_set_from_file(GTK_IMAGE(data->person_image),"pic/man.png");
                gtk_label_set_text(GTK_LABEL(data->main_gender_label),"Мужской");

            }
            struct stat st = {0};
            if (stat("../../data", &st) == -1) {
                mkdir("../../data", 0755);
            }
            
            if (stat("../../data/logs", &st) == -1) {
                mkdir("../../data/logs", 0755);
            }
            char *filename = malloc(sizeof(char)*256);
            if(filename!=NULL){
                snprintf(filename, sizeof(char)*256, 
                        "../../data/logs/operations_%s.json", data->current_card->card_number);
                data->file_history_name=filename;
            }
            snprintf(balance_str, sizeof(balance_str), "%.2fР", balance);
            gtk_label_set_text(GTK_LABEL(data->main_balance_label), balance_str);
            gtk_label_set_text(GTK_LABEL(data->main_card_label), data->current_card->card_number);
            gtk_label_set_text(GTK_LABEL(data->label_user),(data->current_card->holder_name));
            start_fade_transition(data);
            

        }
        else if(card->pin_atempts==0){
            g_print("Количество попыток закончено, карта заблокирована\n");
            card->is_blocked=1;
        }
        else{
            char message[500];
            card->pin_atempts-=1;
            snprintf(message, 500, "Неправильный PIN-код\n Оставшееся количество попыток для входа: %d", card->pin_atempts);
            show_alert(data->main_window, message);
            gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");


            if(card->pin_atempts!=0){
                g_print("Количество попыток оставшееся для входа: %d\n", card->pin_atempts);
            }
        }
    }else{
        printf("Карта не найдена\n");
        show_alert(data->main_window,"Карта не найдена");
        gtk_editable_set_text(GTK_EDITABLE(data->Entry_login), "");
        gtk_editable_set_text(GTK_EDITABLE(data->Entry_pass), "");

    }
}

static void app_activate (GtkApplication *app, gpointer user_data) {
    load_css();
    
    GtkBuilder *builder = gtk_builder_new_from_file("builder.ui");
    GObject *window = gtk_builder_get_object(builder, "window");

    AppData *data = g_new(AppData, 1);
    data->data_path="../data/test_database.json";
    Database *db = create_database();
    if (db) {
        g_print("Загружаем базу данных...\n");
        if (!load_Database_from_json(data->data_path, db)) {
            g_print("Не удалось загрузить базу данных\n");

        } else {
            g_print("База данных загружена успешно\n");
        }
    }
    data->db=db;
    data->main_window = GTK_WINDOW(window);
    data->main_stack = GTK_WIDGET(gtk_builder_get_object(builder, "main-stack"));
    gtk_stack_set_transition_type(GTK_STACK(data->main_stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    gtk_stack_set_transition_duration(GTK_STACK(data->main_stack), 200);
    data->Entry_login=GTK_WIDGET(gtk_builder_get_object(builder, "login-entry"));
    data->Entry_pass=GTK_WIDGET(gtk_builder_get_object(builder,"pass-entry"));
    data->balance_label = GTK_WIDGET(gtk_builder_get_object(builder, "balance-label"));
    data->login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login_page"));
    data->cabinet_page = GTK_WIDGET(gtk_builder_get_object(builder, "cabinet_page"));
    data->last_button=GTK_WIDGET(gtk_builder_get_object(builder, "last_button"));
    data->main_balance_label=GTK_WIDGET(gtk_builder_get_object(builder,"main-balance-label"));
    data->label_user=GTK_WIDGET(gtk_builder_get_object(builder, "label_user"));
    data->main_card_label=GTK_WIDGET(gtk_builder_get_object(builder,"main_card_label"));
    data->current_card=NULL;
    data->button = GTK_WIDGET(gtk_builder_get_object(builder, "Exit-button"));
    data->animation_page = GTK_WIDGET(gtk_builder_get_object(builder,"animation_page"));
    data->transfer_button = GTK_WIDGET(gtk_builder_get_object(builder,"transfer_button"));
    data->current_mode=LOGIN_MODE;
    data->choice_button_card=GTK_WIDGET(gtk_builder_get_object(builder,"choice_button_card"));
    data->choice_button_login=GTK_WIDGET(gtk_builder_get_object(builder,"choice_button_login"));
    data->choice_button_phone=GTK_WIDGET(gtk_builder_get_object(builder, "choice_button_phone"));
    data->take_money_button=GTK_WIDGET(gtk_builder_get_object(builder,"take_money_button"));
    data->give_money_button=GTK_WIDGET(gtk_builder_get_object(builder,"give_money_button"));
    data->give_money_button_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"give_money_button_dialog"));
    data->give_money_entry_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"give_money_entry_dialog"));
    data->take_money_dialog=GTK_DIALOG(gtk_builder_get_object(builder,"take_money_dialog"));
    data->translate_dialog=GTK_DIALOG(gtk_builder_get_object(builder,"translate_dialog"));
    data->give_money_dialog=GTK_DIALOG(gtk_builder_get_object(builder,"give_money_dialog"));
    data->person_image=GTK_WIDGET(gtk_builder_get_object(builder, "person_image"));
    data->main_gender_label=GTK_WIDGET(gtk_builder_get_object(builder,"main_gender_label"));
    data->take_money_button_dialog=GTK_WIDGET(gtk_builder_get_object(builder, "take_money_button_dialog"));
    data->take_money_entry_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "take_money_entry_dialog"));
    data->translate_entry_card_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"translate_entry_card_dialog"));
    data->translate_entry_money_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"translate_entry_money_dialog"));
    data->translate_button_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"translate_button_dialog"));
    data->transfer_button_change=GTK_WIDGET(gtk_builder_get_object(builder,"transfer_button_change"));
    data->current_transfer_mode=TRANSFER_CARD_MODE;
    data->history_button = GTK_WIDGET(gtk_builder_get_object(builder,"history_button"));
    data->file_history_name=NULL;
    data->history_labels_container=GTK_WIDGET(gtk_builder_get_object(builder,"history_labels_container"));
    gtk_widget_set_cursor_from_name(data->button, "pointer");
    gtk_widget_set_cursor_from_name(data->last_button, "pointer");
    gtk_widget_set_cursor_from_name(data->choice_button_phone, "pointer");
    gtk_widget_set_cursor_from_name(data->choice_button_card, "pointer");
    gtk_widget_set_cursor_from_name(data->last_button, "pointer");
    gtk_widget_set_cursor_from_name(data->history_button,"pointer");
    gtk_widget_set_cursor_from_name(data->transfer_button, "pointer");
    gtk_widget_set_cursor_from_name(data->take_money_button, "pointer");
    gtk_widget_set_cursor_from_name(data->give_money_button, "pointer");
    gtk_widget_set_cursor_from_name(data->translate_button_dialog, "pointer");
    gtk_widget_set_cursor_from_name(data->transfer_button_change, "pointer");
    gtk_widget_set_cursor_from_name(data->take_money_button_dialog,"pointer");
    gtk_widget_set_cursor_from_name(data->give_money_button_dialog, "pointer");

    g_signal_connect(window, "destroy",G_CALLBACK(close_and_save),data);
    g_signal_connect(data->last_button, "clicked", G_CALLBACK(click_last),data);
    g_signal_connect(data->button, "clicked", G_CALLBACK(button_clicked), data);
    g_signal_connect(data->choice_button_card, "clicked", G_CALLBACK(button_change_enter_card),data);
    g_signal_connect(data->choice_button_login, "clicked", G_CALLBACK(button_change_enter_login),data);
    g_signal_connect(data->choice_button_phone, "clicked", G_CALLBACK(button_change_enter_phone), data);
    g_signal_connect(data->transfer_button, "clicked", G_CALLBACK(show_dialog_transfer), data);
    g_signal_connect(data->take_money_button, "clicked", G_CALLBACK(show_dialog_take_money), data);
    g_signal_connect(data->translate_dialog, "close-request", 
                     G_CALLBACK(on_dialog_close_request), data);
    g_signal_connect(data->take_money_dialog, "close-request", 
                     G_CALLBACK(on_dialog_close_request), data);

    g_signal_connect(data->give_money_button, "clicked", G_CALLBACK(show_give_money_dialog),data);
    g_signal_connect(data->give_money_dialog, "close-request", 
                     G_CALLBACK(on_dialog_close_request), data);
    

    g_signal_connect(data->give_money_button_dialog,"clicked",G_CALLBACK(balance_up_dialog_button),data);
    g_signal_connect(data->take_money_button_dialog,"clicked",G_CALLBACK(balance_low_dialog_button),data);
    g_signal_connect(data->translate_button_dialog, "clicked", G_CALLBACK(balance_translate_dialog_button),data);
    g_signal_connect(data->transfer_button_change,"clicked",G_CALLBACK(transfer_button_change_press),data);
    g_signal_connect(data->history_button,"clicked",G_CALLBACK(open_log_file),data);





    g_signal_connect(data->Entry_login, "changed", G_CALLBACK(on_text_changed), data);
    g_object_unref(builder);

    gtk_stack_set_visible_child(GTK_STACK(data->main_stack), data->login_page);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));
}
int main (int argc, char **argv)
{
    GtkApplication *app = gtk_application_new ("com.metanit", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);
 
    return status;
}