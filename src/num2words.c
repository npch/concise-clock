#include "num2words.h"
#include "string.h"

static const char* const ONES[] = {
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TEENS[] ={
  "",
  "eleven",
  "twelve",
  "thirteen",
  "fourteen",
  "fifteen",
  "sixteen",
  "seventeen",
  "eighteen",
  "nineteen"
};

static const char* const TENS[] = {
  "",
  "ten",
  "twenty",
  "thirty",
  "forty",
  "fifty",
  "sixty",
  "seventy",
  "eighty",
  "ninety"
};

static const char* const CONCISETERMS[] = {
  "Almost",
  "Nearly",
  "Exactly",
  "Slightly after"
  "Just after"
};

static const char* const BRITTERMS[] = {
  "Getting on for",
  "Tad before",
  "Bang on",
  "Smidge after",
  "Trifle after"
};

static const char* const HOURS[] = {
  "midnight",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine",
  "ten",
  "eleven",
  "midday",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine",
  "ten",
  "eleven"
};

static const char* const INTERVALS[] = {
  "",                 // [0]  58 - 02 = H O'Clock
  "five past",        // [1]  03 - 07 = five past H
  "ten past",         // [2]  08 - 12 = ten past H
  "quarter past",     // [3]  13 - 17 = quarter past H
  "twenty past",      // [4]  18 - 22 = twenty past H
  "twenty five past", // [5]  23 - 27 = twenty five past H
  "half past",        // [6]  28 - 32 = half past H
  "twenty five to",   // [7]  33 - 37 = twenty five to H
  "twenty to",        // [8]  38 - 42 = twenty to H
  "quarter to",       // [9]  43 - 47 = quarter to H
  "ten to",           // [10] 48 - 52 = ten to H
  "five to"           // [11] 53 - 57 = five to H
};

static const char* STR_OH_CLOCK = "o'clock";
//static const char* STR_NOON = "noon";
//static const char* STR_MIDNIGHT = "midnight";
//static const char* STR_QUARTER = "quarter";
//static const char* STR_TO = "to";
//static const char* STR_PAST = "past";
//static const char* STR_HALF = "half";
//static const char* STR_AFTER = "after";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5); // truncate towards zero
  int fuzzy_terms = (minutes + 2) % 5;


  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 11) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
  } else if (fuzzy_minutes > 6 && fuzzy_minutes < 12 ) {
    fuzzy_hours +=1;
  }
  if (fuzzy_hours > 23) {
    fuzzy_hours = 0;
  }

  size_t remaining = length;
  memset(words, 0, length);

  if (fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, BRITTERMS[fuzzy_terms]);
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, HOURS[fuzzy_hours]);
    if(fuzzy_hours != 0 && fuzzy_hours != 12) {
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_OH_CLOCK);
    }
  } else {
    remaining -= append_string(words, remaining, BRITTERMS[fuzzy_terms]);
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, INTERVALS[fuzzy_minutes]);
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, HOURS[fuzzy_hours]);
  }

}
