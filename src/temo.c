#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct student
{
    int rno;
    char name[20];
};


struct node
{
    struct student data;
    struct node *next;
};


void readStudentDetails(struct student *);
void addAtBeg(struct node **, struct student);
struct student deleteAtBeg(struct node **);


int main()
{
    struct node *head = NULL;
    int ch, f, op, i;
    struct student e;
    char name[20];


    while(1)
    {
        printf("1.Add node at beginning\n");
        printf("2.Delete node at beginning\n");
        printf("3.Search Student by name\n");
        printf("4.Display Student List\n");
        printf("5.Exit\n");

        printf("Enter Choice: ");
        scanf("%d",&ch);

        switch(ch)
        {
            case 1: readStudentDetails(&e);
                    addAtBeg(&head, e);
                    break;

            case 2: printf("Deleted data:\n");
                    e = deleteAtBeg(&head);
                    printf("Roll NO: %d\t",e.rno);
                    printf("Name: %s\n\n",e.name);
                    break;

            case 3: printf("Enter name: ");
                    scanf("%s",name);
                    i = searchName(head, name);
                    printf("%d\n\n",i);
                    break;

            case 4: printf("ROLL NO\tNAME\n");
                    displayList(head);
                    break;


            case 5: exit(0);
                break;

        }
    }
}



void displayList(struct node *head)
{
    struct node *p = head;

    while(p)
    {
        printf("rno: %d\t",p->data.rno);
        printf("name: %s\n",p->data.name);
        p = p->next;
    }
    /*while(p)
    {
        displayStudentDetails(p->data);
        p = p->next;
    }*/

}





void displayStudentDetails(struct student *s)
{
    printf("%d\t%s\n",s->rno,s->name);
}




int searchName(struct node *head, char name[])
{
        struct node *p = head;

        while(p)
        {
            if (!(strcasecmp((p->data).name, name)))
                return 1;

            p = p->next;

        }
        return 0;
}


struct student deleteAtBeg(struct node **head)
{
    struct node *p = *head;
    struct student e;

    e = p->data;
    *head = (*head)->next;
    free(p);
    return e;

}


void addAtBeg(struct node **head, struct student e)
{
    struct node *p;
    p = (struct node *)malloc (sizeof(struct node));

    if(p==NULL)
    {
        perror("No memoery available");
        return;
    }

    p->data = e;
    p->next = *head;
    *head = p;
}

    DDT *cur = root;
    while (cur->left != NULL)
    {
        cur = cur->left;
    }

    printf("%s\t%d\t%lf\t%d\t%lf\t%s\n",
           cur->data->dormitoryName,
           cur->data->dormitoryBedCount,
           cur->data->dormitoryRent,
           cur->data->dormitoryAvailability,
           cur->data->dormitoryRating,
           cur->data->description);

    // Free allocated memory
