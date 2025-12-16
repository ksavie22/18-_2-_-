#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void print_line(const char* str, char fill) {
    int len = strlen(str);
    int left = (80 - len) / 2;
    int right = 80 - len - left;

    for (int i = 0; i < left; i++) putchar(fill);
    printf("%s", str);
    for (int i = 0; i < right; i++) putchar(fill);
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "RUS");
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    char* weekdays[] = { "воскресенье", "понедельник", "вторник", "среда",
                       "четверг", "пятница", "суббота" };
    char* months[] = { "января", "февраля", "марта", "апреля", "мая", "июня",
                     "июля", "августа", "сентября", "октября", "ноября", "декабря" };
    char* roman[] = { "I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII" };
    char* seasons[] = { "зима", "весна", "лето", "осень" };

    int h = tm->tm_hour, m = tm->tm_min, s = tm->tm_sec;
    int day = tm->tm_mday, mon = tm->tm_mon, wday = tm->tm_wday;
    int year = tm->tm_year + 1900, yday = tm->tm_yday;
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    print_line("", '*');
    print_line("ТЕКУЩЕЕ ВРЕМЯ И ДАТА", '*');
    print_line("", '*');

    char buf[100];

    sprintf(buf, "Московское время %02d:%02d:%02d", h, m, s);
    print_line(buf, ' ');

    sprintf(buf, "Сегодня: %s", weekdays[wday]);
    print_line(buf, ' ');

    sprintf(buf, "Дата: %02d.%02d.%d", day, mon + 1, year);
    print_line(buf, ' ');

    sprintf(buf, "%d %s", day, months[mon]);
    print_line(buf, ' ');

    sprintf(buf, "Сегодня %d-й день %d года", yday + 1, year);
    print_line(buf, ' ');

    int to_sun = (7 - wday) % 7;
    int h_to_sun = to_sun * 24 - h;
    if (h_to_sun < 0) h_to_sun += 24;
    sprintf(buf, "До воскресенья: %d дней и %d часов", to_sun, h_to_sun);
    print_line(buf, ' ');

    int from_pair_h = h - 8;
    int from_pair_m = m - 30;
    if (from_pair_m < 0) { from_pair_h--; from_pair_m += 60; }
    if (from_pair_h < 0) from_pair_h += 24;
    sprintf(buf, "С первой пары (8:30) прошло: %d часов %d минут", from_pair_h, from_pair_m);
    print_line(buf, ' ');

    int to_pair_m = (10 - h) * 60 - m;
    if (to_pair_m < 0) to_pair_m += 1440;
    int to_pair_s = to_pair_m * 60 - s;
    sprintf(buf, "До конца пары (10:00): %d минут %d секунд", to_pair_s / 60, to_pair_s % 60);
    print_line(buf, ' ');

    int month_days[] = { 31,28 + leap,31,30,31,30,31,31,30,31,30,31 };
    int to_month_end = month_days[mon] - day;
    int to_year_end = 11 - mon;
    sprintf(buf, "До конца месяца: %d дней, до конца года: %d месяцев", to_month_end, to_year_end);
    print_line(buf, ' ');

    struct tm ny = *tm;
    ny.tm_year++; ny.tm_mon = 0; ny.tm_mday = 1; ny.tm_hour = ny.tm_min = ny.tm_sec = 0;
    time_t ny_t = mktime(&ny);
    int to_ny = difftime(ny_t, t) / 86400;
    sprintf(buf, "До Нового года: %d месяцев и %d дней", to_ny / 30, to_ny % 30);
    print_line(buf, ' ');

    sprintf(buf, "%s %d", roman[mon], year);
    print_line(buf, ' ');

    int season = (mon == 11 || mon <= 1) ? 0 : (mon >= 2 && mon <= 4) ? 1 : (mon >= 5 && mon <= 7) ? 2 : 3;
    sprintf(buf, "%s, %02d год", seasons[season], year % 100);
    print_line(buf, ' ');

    int season_day = day;
    if (mon == 11) season_day = day;
    else if (mon == 0) season_day = day + 31;
    else if (mon == 1) season_day = day + 31 + 31;
    else if (mon >= 2 && mon <= 4) {
        for (int i = 2; i < mon; i++) season_day += month_days[i - 1];
    }
    else if (mon >= 5 && mon <= 7) {
        for (int i = 5; i < mon; i++) season_day += month_days[i - 1];
    }
    else {
        for (int i = 8; i < mon; i++) season_day += month_days[i - 1];
    }

    sprintf(buf, "Идет %d день %s", season_day, seasons[season]);
    print_line(buf, ' ');

    print_line("", '*');

    return 0;
}