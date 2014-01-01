#include <stdio.h>

typedef struct {
  char *name;
  char gender;
  int  age;
  float weight, height;
} Person;

void person2xml(const Person *p, char *xml) {
  sprintf(xml, "<person name=\"%s\" gender=\"%c\">\n <age>%d</age>\n <weight>%.2f</weight>\n <height>%.2f</height>\n</person>\n",  p->name, p->gender, p->age, p->weight, p->height);
}

int main()
{
  Person list[] = {{ .name="John", .gender='M', .age=45, .weight=59.5, .height=167.5 }, 
                   { .name="Mary", .gender='F', .age=22, .weight=52.3, .height=160.7 }};
				
  char listXml[1000];
  int i, tail = 0;				
  for (i=0; i<sizeof(list)/sizeof(Person); i++) {
	person2xml(&list[i], &listXml[tail]);
	tail = strlen(listXml);
  }
  printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<PersonList>\n%s</PersonList>", listXml);
}
