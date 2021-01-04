/* 
Created on 18/11/2019 by Fletcher van Ameringen

File Line Format:
	number;name name;A1:A1_worth;A2_mark:A2_worth;A3_mark:A3_worth;#exam_worth
	or
	string;string;float:float;float:float;float:float;#int
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void get_name(char *lin, char *nam);
int get_asmt_number(char *lin);
int get_asmts(char *lin, int a_num);
float grade_est(float num, float total);

float asmt_grade[50], asmt_worth[50];

int main() {
	int i, indx = -1;
	char num_compare[50], line[500];
	
	FILE *f;
	
	int asmt_number = 0, exam_worth = 100;
	char num[50], paper_name[50];
	
	printf("Enter paper number (with period): ");
	fgets(num, 50, stdin);
	num[strlen(num)-1] = '\0';
	f = fopen("papers.txt", "r");
	
	if (f == NULL) {
		printf("Could not open papers.txt\n");
		exit(0);
	}

	while (fgets(line, 500, f) != NULL) {
		if (line[0] == '#') {
			continue; // This line is a comment
		}
		line[strlen(line)-1] = '\0'; // turn \n into \0
		i = 0;

		while (line[i] != ';') {
			num_compare[i] = line[i];
			i++;
		}
		num_compare[i] = '\0';
		
		if (strcmp(num_compare, num) == 0) {
			indx++;
			get_name(line, paper_name);
			asmt_number = get_asmt_number(line);
			exam_worth = get_asmts(line, asmt_number);
			break;
		}
	}	
	fclose(f);
	if (indx == -1) {
		printf("No paper could be found with this name.\n");
		exit(0);
	}
	float total = 0.0, col_worth = 0.0, avg = 0.0;
	
	for (i = 0; i < asmt_number; i++) {
		total += asmt_grade[i];
	}
	avg = total / asmt_number;
	
	for (i = 0; i < asmt_number; i++) {
		col_worth += asmt_worth[i];
	}
	
	if ((col_worth >= 50.0) && (total >= 50.0)) {
		printf("You have passed this paper already (due to assignment weighting)!\n)");
	} else {
		printf("You have not passed this paper yet\n");
	}
	
	//printing
	printf("\n");
	for (i = 0; i < asmt_number; i++) {
		printf("A%d Mark: %.2f\n", i+1, asmt_grade[i]);
		printf("A%d Worth: %.2f\n", i+1, asmt_worth[i]);
		printf("\n");
	}	
	printf("Paper Number: %s\n", num);
	printf("Paper Name: %s\n\n", paper_name);
	printf("Exam Worth: %d%%\n", exam_worth);
	printf("Average Assignment Mark: %.2f%%\n", avg);
	printf("Assignment Collective Marks: %.2f%% / %.2f%%\n", total, col_worth);
	printf("Grade needed from exam to achieve each letter grade...\n\n");
	
	printf("A+\t:\t%.2f / %d\n", grade_est(90.0, total), exam_worth);
	printf("A\t:\t%.2f\n", grade_est(85.0, total));
	printf("A-\t:\t%.2f\n", grade_est(80.0, total));
	printf("B+\t:\t%.2f\n", grade_est(75.0, total));
	printf("B\t:\t%.2f\n", grade_est(70.0, total));
	printf("B-\t:\t%.2f\n", grade_est(65.0, total));
	printf("C+\t:\t%.2f\n", grade_est(60.0, total));
	printf("C\t:\t%.2f\n", grade_est(55.0, total));
	printf("C-\t:\t%.2f\n", grade_est(50.0, total));
}

void get_name(char *lin, char *name) {
	int i = 0, j = 0;
	char temp[500];
		
	while (lin[i] != ';') {
		i++;	
	}
	i++; // skip ';'
	
	while (lin[i] != ';') {
		name[j] = lin[i];
		i++;	
		j++;
	}
	name[j] = '\0';
	i++; // skip ';'
	j = 0;
	
	while (lin[i] != '\0') {
		temp[j] = lin[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	strcpy(lin, temp);
	/* 
	Now the format is:
	A1:A1_worth;A2_mark:A2_worth;A2_mark:A2_worth;#exam_worth
	*/
}

int get_asmt_number(char *lin) {
	int i = 0, asmts = 0;
	
	while (lin[i] != '#') {
		if (lin[i] == ':') {
			asmts++;
		}
		i++;
	}
	return asmts;
}

int get_asmts(char *lin, int a_num) {
	int i, j = 0, count = 0, exam, ex = 0;
	
	char temp[50];

	while (j < a_num) {
		i = 0;
		while (lin[ex] != ';') {
			temp[i] = lin[ex];
			i++;
			ex++;
		}
		temp[i] = '\0';
		ex++; // skip ';'
		sscanf(temp, "%f%*c%f", &asmt_grade[count], &asmt_worth[count]);
		count++;
		j++;
	}	
	// load exam_worth
	ex++; //skip '#'
	j = 0;
	
	while (lin[ex] != '\0') {
		temp[j] = lin[ex];
		ex++;
		j++;
	}
	temp[j] = '\0';
	sscanf(temp, "%d", &exam);
	return exam;
	
}
float grade_est(float num, float total) {
	if (num > total) {
		return num - total;
	} else {
		return 0.0;
	}
}