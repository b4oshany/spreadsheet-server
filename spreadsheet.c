/**
* @author:      Oshane Bailey
* @id:          620042528
* @proejct:     Assignement 1
* @lecturer:    Kevin Miller
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define COL_SIZE 9
#define ROW_SIZE 9
int largest_cell = 0;

struct Cell{
    int pos[2];
    char input[100];
    char type[10];
    int intVal;
    char strVal[100];
};

struct Cell sheet[ROW_SIZE][COL_SIZE];

#define numeric(str) ( (int) strtol(str, (char **)NULL, 10) )

int is_cell_string(int r, int c){
  return strcmp(sheet[r][c].type, "string")==0;
}
int is_cell_numeric(int r, int c){
  return strcmp(sheet[r][c].type, "numeric")==0;
}
int is_cell_func(int r, int c){
  return strcmp(sheet[r][c].type, "function")==0;
}
void upper_string(char s[]) {
   int c = 0;
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
}

int calc_result(char func[], int data[], int count){
  int i, result = 0;
  upper_string(func);
  if(strcmp(func, "SUM")==0){
    for(i=0; i < count; i++){
      result += data[i];
    }
    return result;
  }else if(strcmp(func, "AVERAGE")==0){
    return calc_result("SUM", data, count)/count;
  }else if(strcmp(func, "RANGE")==0){
    for(i=0; i < count; i++){
      if(data[i] >= result){
        result = data[i];
      }
    }
  }else{
    printf("No such function exists: %s", func);
  }
  return result;
}

void get_cell_points(char str[], int a, int result[]){
  if(str[a] >= 65 && str[a] <= 73 && str[a+1] >= 49 && str[a+1] <= 57){
    result[1] = str[a] - 65;
    result[0] = str[a+1] - 49;
    result[2] = a+1;
  }else if(str[a] >= 97 && str[a] <= 105 && str[a+1] >= 49 && str[a+1] <= 57){
    result[1] = str[a] - 97;
    result[0] = str[a+1] - 49;
    result[2] = a+1;
  }else{
    result[0] = -1;
  }
}

void update_cell_by_func(int r, int c, int ln){
  int a, has_params = 0, pos[3], rPos, cPos, nCell = 0, iSkip = 0, iData = 0, isSingular = 1;
  int data[100], count = 0;
  char func[10] = "", str[100] = "";
  strcpy(str, sheet[r][c].input);
  for(a=1; a<ln; a++){
    if(a <= iSkip){
      continue;
    }
    if(has_params != 1 && str[a] == 40){
      has_params = 1;
      continue;
    }else if(has_params != 1 && str[a] != 40){
      func[strlen(func)] = str[a];
    }else if(has_params == 1 && str[a] != 41 && (a+1) < ln){
      get_cell_points(str, a, pos);
      if(pos[0] != -1){
        iSkip = pos[2];
        cPos = pos[1];
        rPos = pos[0];
      }else if(str[a] == ','){
        isSingular = 1;
        if(is_cell_numeric(rPos, cPos)){
          data[count++] = sheet[rPos][cPos].intVal;
        }else if(is_cell_func(rPos, cPos)){

        }else{
          printf("Invalid operation on string\n");
          break;
        }
      }else if(str[a] == ':' && (a+2) < ln){
        get_cell_points(str, a+1, pos);
        for(rPos=rPos; rPos <= pos[0]; rPos++){
          for(cPos=cPos; cPos <= pos[1]; cPos++){
            data[count++] = sheet[rPos][cPos].intVal;
          }
        }
        cPos = pos[1];
        rPos = pos[0];
        iSkip = pos[2];
      }
    }else if(has_params == 1 && str[a] == 41){
      data[count++] = sheet[rPos][cPos].intVal;
      if((a+1) < ln){
        printf("Invalid operation on string\n");
      }
      break;
    }
  }
  sheet[r][c].intVal = calc_result(func, data, count);
}

/**
* Print the Column headings
* @param spaces {char[]} - space characters to space each heading character by.
*/
void print_heading(char spaces[]){
  char col[] = "ABCDEFGHI"; // heading characters
  char str[strlen(spaces)*2*strlen(col) + 2]; // heading string
  int i;
  sprintf(str, "|"); // added the left cell border of the row.
  // append each heading character with space to the heading string.
  for(i=0; i < strlen(col); i++){
    sprintf(str, "%s%s%c%s|", str, spaces, col[i], spaces);
  }
  // print the spreadsheet heading
  printf("   |   ");
  printf("%s\n", str);
}

void print_dash(int num_space){
  int num_dash = 24, i;
  int total_dash = num_dash + (2 * num_space * 9);
  // printf("dash %d\n", total_dash);
  printf("   ");
  for(i=0; i < total_dash - 1; i++){
    printf("-");
  }
  printf("\n");
}

void print_str_cell(char data[], char spaces[]){
  int n_str, t_str, i;
  printf("%s%s%s", spaces, data, spaces);
}

void print_int_cell(int data, char spaces[]){
  char str[1000];
  sprintf(str, "%d", data);
  print_str_cell(str, spaces);
}


/**
* Print a specific number of rows.
* @param num_rows {int} - Number of rows to print.
*/
void print_rows(int num_rows){
  int a, b, i, ln, d_space = 2, num_side_space = 0;
  num_side_space = largest_cell/2 + d_space;
  // printf("n sides: %d\n", num_side_space);
  char spaces[num_side_space];
  char default_space = ' ';
  if(num_side_space > 1){
    for(i=0; i < num_side_space; i++){
      sprintf(spaces, " %s", spaces);
    }
  }else{
    sprintf(spaces, " ");
  }
  num_side_space = strlen(spaces);
  // printf("l of s: %d\n", num_side_space);
  // printf("lc : %d\n", largest_cell);
  print_dash(num_side_space);
  print_heading(spaces);
  print_dash(num_side_space);
  for (a = 0; a < num_rows; a++) {
     printf("   | %d |", a+1);
     for (b = 0 ; b < COL_SIZE; b++) {
        if(is_cell_numeric(a, b)){
          print_int_cell(sheet[a][b].intVal, spaces);
        }else if(is_cell_func(a, b)){
          ln = strlen(sheet[a][b].input);
          update_cell_by_func(a, b, ln);
          print_int_cell(sheet[a][b].intVal, spaces);
        }else if(is_cell_string(a, b)){
          // side_space = (d_space + cell_space - strlen(sheet[a][b].input))/2;
         print_str_cell(sheet[a][b].input, spaces);
       }else{
         print_str_cell(" ", spaces);
       }
       printf("|");
     }
     printf("\n");
     print_dash(num_side_space);
  }
}

/**
* Print all rows and columns.
*/
void print_cells(){
  print_rows(ROW_SIZE);
}



void update_cell(int r, int c){
  int val = numeric(sheet[r][c].input), ln, a;
  ln = strlen(sheet[r][c].input);
  if(ln > 0){
    if(sheet[r][c].input != "0" && val != 0){
      strcpy(sheet[r][c].type, "numeric");
      sheet[r][c].intVal = val;
    }else if(sheet[r][c].input[0] == '='){
      update_cell_by_func(r, c, ln);
      strcpy(sheet[r][c].type, "function");
    }else{
      strcpy(sheet[r][c].type, "string");
    }
  }
}

/**
* Capture the input from the user and update the spreadsheet data.
*/
void capture_cells(){
 int row = 0, // cell row
  col = 0, // cell column
  n_str, // string length
  str_end = 0; // ending of a string
 char c, delimiter = ' ';
 printf("Enter each cell separated by space (for columns) and new lines (for rows): \n");
 do{
   scanf("%c", &c);
   // If a space occurs, update the sheet and capture a new cell data.
   if(c == delimiter){
     update_cell(row, col);
     col++;
     str_end = 0;
   }else if(c == '\n'){
     update_cell(row, col);
     row++;
     str_end++;
     col=0;
     print_rows(row);
   }else{
     sprintf(sheet[row][col].input, "%s%c", sheet[row][col].input, c);
     n_str = strlen(sheet[row][col].input);
     if(n_str > largest_cell){
       largest_cell = n_str;
     }
   }
 }while(str_end != 2);
}

/**
* Save the file as a csv file.
*/
void saveWorksheet(){
  int a, b; // counters
  char line[1000], // store each row of the spreadsheet.
   filename[50], // name of the file to store the spreadsheet.
   choice; // choice to store the spreadsheet with or without the formula.
  FILE *fpt; // file pointer to be used to store the spreadsheet

  // Prompt the users for the file name.
  printf("What do you want to save the file as?:\n");
  scanf("%s", filename);
  sprintf(filename, "%s.csv", filename);
  printf("This spreadsheet will be saved in %s\n\n", filename);

  // Ask the user if the spreadsheet should be stored with the formula or not.
  printf("Save the file with the formula: (Y or N)\n");
  scanf("%c", &choice);

  // Check for valid choice for
  while(choice != 'Y' && choice != 'N'){
    if(choice != 'Y' && choice != 'N'){
      printf("Invalid option, answer Y or N to save the spreadsheet with formula (enter C to cancel):\n");
      scanf("%c", &choice);
      if(choice == 'C'){
        return;
      }
    }
  }

  fpt=fopen(filename, "w"); // open the file

  // Build the spreadsheet for CSV output.
  // Iterate over each row in the sheet
  for (a = 0; a < ROW_SIZE; a++) {
    // Iterate over each column of each row in the sheet.
    sprintf(line, ""); // ensure the line is not empty
    for (b = 0 ; b < COL_SIZE; b++) {
      // Check if the cell is a numeric, a function or string value, and
      // append it to the line variable accordingly
       if(is_cell_numeric(a, b)){
         sprintf(line, "%s%d,", line, sheet[a][b].intVal);
       }else if(is_cell_func(a, b)){
         // Check if the user indicated that the formula should be printed,
         // if yes, then output the formula, else output the integer value.
         if(choice == 'Y'){
           sprintf(line, "%s%s,", line, sheet[a][b].input);
         }else{
           sprintf(line, "%s%d,", line, sheet[a][b].intVal);
         }
       }else if(is_cell_string(a, b)){
         sprintf(line, "%s%s,", line, sheet[a][b].input);
       }else{
         // Put an empty string if nothing was found.
         sprintf(line, "%s,", line);
       }
    }
    // append a new line with data to the file.
    fprintf(fpt, "%s\n", line);
  }
  // close the file.
  fclose(fpt);
}


/**
* Open csv file as a spreadsheet.
* @param filename {char[]} - name of the csv file.
*/
void openWorksheet(char filename[]){
  int row = 0, col = 0, i, // counters
    n_chars = 10000, // number of characters in the file.
    n_str = 0; // number of characters in the scoped string.
  char lines[n_chars], // store each row of the spreadsheet.
   delimiter = ',', // delimiter for the csv file.
   str[] = ""; // choice to store the spreadsheet with or without the formula.
  FILE *fpt; // file pointer to be used to store the spreadsheet

  fpt=fopen(filename, "r"); // open the file.
  fread(lines, n_chars, 1, fpt); // read in the content of the file.
  for(i=0; i<strlen(lines); i++){
    // Ignore all carriage return.
    if(lines[i] == '\r'){
      continue;
    }
    if(lines[i] == delimiter){
      // the character is a delimiter, for instance a comma, then move to
      // new column in the current row.
      update_cell(row, col);
      col++;
    }else if(lines[i] == '\n'){
      // if the character is a new line, then move to the next row
      // within the spreadsheet.
      update_cell(row, col);
      row++;
      col=0;
    }else{
      // Append the current character to the current cell.
      sprintf(sheet[row][col].input, "%s%c", sheet[row][col].input, lines[i]);
      // Check the length of the string in the cell, if it's greater than
      // the largest string record, set it as the largest string recorded.
      n_str = strlen(sheet[row][col].input);
      if(n_str > largest_cell){
        largest_cell = n_str;
      }
    }
  }
}

/**
* Print out the options for help.
*/
void print_help(){
  printf("Avaible options:\n");
  printf("- S \t Save a current spreadsheet.\n");
  printf("- O \t Open a csv file.\n");
  printf("- C \t To quit the application\n");
  printf("- H \t For help\n");
  printf("\n");
}

main(int argc, char *argv[]){
  char str[30], val[30], filename[50], choice;
  int pos[3];

  if(argc < 2){
    print_cells();
    capture_cells();
  }else{
    if( access( argv[1], F_OK ) != -1 ) {
      openWorksheet(argv[1]);
    }else{
      printf("\n Error:\n The %s doesn't exists\n", argv[1]);
      return;
    }
  }
  print_cells();
  print_help();
  do{
    printf("\nEnter the cell you want to update or any available options\n");
    scanf("%s", str);
    if(strcmp(str, "C")==0){
      break;
    }else if(strcmp(str, "S")==0){
      saveWorksheet();
    }else if(strcmp(str, "H")==0){
      print_help();
    }else if(strcmp(str, "O")==0){
      printf("\nEnter the name of the file you wish to open\n");
      scanf("%s", filename);
      if( access( filename, F_OK ) != -1 ) {
        openWorksheet(filename);
      }else{
        printf("\n Error:\n The %s doesn't exists\n", filename);
        return;
      }
    }else{
      get_cell_points(str, 0, pos);
      if(pos[0] != -1){
        printf("Enter value for %s:\n", str);
        scanf("%s", val);
        sprintf(sheet[pos[0]][pos[1]].input, "%s", val);
        update_cell(pos[0], pos[1]);
        print_cells();
      }else{
        printf("No such cell exists, %s\n", str);
        continue;
      }
    }
  }while(1);
}
