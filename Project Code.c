// C MINI PROJECT   Problem Solving With C Laboratory - UE19CS152
// Customer Billing System Mini Project (Application Program)

#include<stdio.h>        //.h header file for input and output functions
#include<stdlib.h>       // this header file is for exit() function
#include<termios.h>  
#include<string.h>   //this header file is for string functions like strcat(),strcpy() etc.

#define USERNAME "sumitadmin"   // defining username to be used in admin login.
#define PASSWORD "sumit123"    // password for admin login

static struct termios old, new;

void initTermios(int echo)
{
     tcgetattr(0,&old);
     new = old;
     new.c_lflag &= ~ICANON;
     new.c_lflag &= echo ? ECHO : ~ECHO;
     tcsetattr(0,TCSANOW,&new);
}

void resetTermios(void)
{
     tcsetattr(0,TCSANOW,&old);     //tcsetattr is an inbuilt function.
}

char getch_(int echo)
{
     char ch;
     initTermios(echo);
     ch = getchar();
     resetTermios();
     return ch;
}

char getch(void)
{
     return getch_(0);
}

char getche(void)
{
     return getch_(1);
}

void getPassword(char *pass)
{
     int c=0;
     char buff[30]={0},ch;
     int len=0;
     while((ch=getch())!='\n')
     {
          if(ch==0x7f)
          {
               if(len==0)
                printf("\b \b");  len-- ; continue;
          }
          printf("%c",'*');
          pass[len]=ch;
          len++;
     }
     pass[len]='\0';
}


struct Customer      //creating a structure of customer which gives the deatils about the each customer 
{
     int due_amount;
     int number;
     int no_of_products;
     struct grocery             // structure which is defined inside a structure (sructure in a structure).
     {
        int no_of_items;
        int total_price;
        char item_name[20];
        int unit_price;
     }product[20];           // product details of each customer 
     char name[20],date[10];
     int paid_amount;
     int bill_amount;
};
void addAccounts();
void searchAccountByName();
void runApplication();    //user defined function to run this application
int  displayMenu();           // to display the menu that the admin wants to perform tasks
void searchAccountBySlNum();
void displayAllCustomers();
void saveCustomers(); 
void loadSavedCustomers();
void flush();


struct Customer customer[100];      //creating array of structures to store each customer details.
int currentSelectedOption = -1;   //global variable ,defined globally
int customerCount = 0;

int main ()             //main function of this program.
{
    char user[30],pass[30];
    int i;
    printf("\n\n************* Admin Login *************\n\n");
    printf("\tEnter username : ");
    gets(user);
    printf("\tEnter password : ");
    getPassword(pass);
    

    if(!strcmp(user,USERNAME) && !strcmp(pass,PASSWORD))
    {
        printf("\n\n         Logged IN Successful   \n");
        loadSavedCustomers();
        while(1) runApplication();
    }
    else 
    {
        printf("\n\nIncorrect username and password!!!!!!!! (please enter correct login credentials) \n");
        main();
    }
    return 0;
}

void runApplication()
{
	//loadSavedCustomers();
    	currentSelectedOption = displayMenu();
    	switch (currentSelectedOption)
    	{
        	case 1:
            	addAccounts();
            break;

        case 2:
            searchAccountByName();
            break;

        case 3:
            searchAccountBySlNum();
            break;

        case 4:
            displayAllCustomers();
            break;

        case 5:
            // TODO: Save and Exit
		  saveCustomers();
            exit(0);
            break;

        default:
            printf("\n\nYou have made a wrong choice. Please retry!\n");
            break;
    }
}

void displayAllCustomers()
{
    for (int i = 0; i < customerCount; i++)
    {
          printf("\n*******************************************************************************************\n");

	     printf("Customer Name           : %s\n",customer[i].name);
	     printf("Customer Number         : %d\n",customer[i].number);
          printf("Date (dd/mm/yyyy)       : %s\n",customer[i].date);
          printf("\nItem                      Unit Price       Quantity       Total Price \n");
        for(int j=0 ; j < customer[i].no_of_products ; j++)
        {
          printf("  %s                              %d             %d                %d ",customer[i].product[j].item_name,customer[i].product[j].unit_price,customer[i].product[j].no_of_items,customer[i].product[j].total_price);
          printf("\n");      
        }
          printf("-------------------------------------------------------------------\n");
          printf("                    Total Bill Amount     :       %d         \n ",customer[i].bill_amount);
          printf("-------------------------------------------------------------------\n");
          printf("Paid Amount                   : %d \n",customer[i].paid_amount);
          printf("-------------------------------------------------------------------\n");          //due amount or the balance amount to be paid by the customer
          printf("Due Amount is                 : %d \n\n ",customer[i].due_amount);
	      printf("-------------------------------------------------------------------\n\n"); 
       
          printf("*******************************************************************************************\n");
          if((customerCount-i)==1)
             return;
    }
    printf("\nThere are no customer . Please retry! \n\n");
}

void searchAccountByName()
{
    char name[100];
    printf("Enter the name to search: ");
    scanf("%s", name);

    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(name, customer[i].name) == 0)
        {
            //printf("Found customer -> ID: %d, Name: %s, Age: %d", customer[i].id, customer[i].name, customer[i].age);
		    printf("\n**************************************************************************************\n");

		    printf("Customer Name           : %s\n",customer[i].name);
	         printf("Customer Number         : %d\n",customer[i].number);
              printf("Date (dd/mm/yyyy)       : %s\n",customer[i].date);
              printf("\nItem                      Unit Price       Quantity       Total Price \n");
             for(int j = 0 ; j<customer[i].no_of_products ; j++)
           {
              printf("  %s                           %d              %d               %d ",customer[i].product[j].item_name,customer[i].product[j].unit_price,customer[i].product[j].no_of_items,customer[i].product[j].total_price);
              printf("\n");      
           }
              printf("-------------------------------------------------------------------\n");
              printf("                    Total Bill Amount      :             %d         \n ",customer[i].bill_amount);
              printf("-------------------------------------------------------------------\n");
              printf("Paid Amount                   : %d \n",customer[i].paid_amount);
              printf("-------------------------------------------------------------------\n");        //due amount or the balance amount to be paid by the customer
              printf("Due Amount is                 : %d \n\n ",customer[i].due_amount);
	         printf("-------------------------------------------------------------------\n\n"); 
            
              printf("**************************************************************************************\n");
              return;
        }
    }
    printf("\n\n Could not find the customer name. Please retry!\n");
}

void searchAccountBySlNum()
{
    int slNum;
    printf("Enter the Sl number to search: ");
    scanf("%d", &slNum);

    for (int i = 0; i < customerCount; i++)
    {
        if (customer[i].number == slNum)
        {
            //printf("Found customer -> ID: %d (sl num: %d), Name: %s, Age: %d", customer[i].id, customer[i].id + 1, customer[i].name, customer[i].age);
		    printf("\n**************************************************************************************\n");
		    printf("Customer Name           : %s\n",customer[i].name);
	         printf("Customer Number         : %d\n",customer[i].number);
              printf("Date (dd/mm/yyyy)       : %s\n",customer[i].date);
              printf("\nItem                      Unit Price       Quantity       Total Price \n");
             for(int j = 0 ; j < customer[i].no_of_products ; j++)
           {
              printf("  %s                           %d              %d               %d ",customer[i].product[j].item_name,customer[i].product[j].unit_price,customer[i].product[j].no_of_items,customer[i].product[j].total_price);
              printf("\n");      
           }
              printf("-------------------------------------------------------------------\n");
              printf("                    Total Bill Amount      :              %d         \n ",customer[i].bill_amount);
              printf("-------------------------------------------------------------------\n");
              printf("Paid Amount                   : %d \n",customer[i].paid_amount);
              printf("-------------------------------------------------------------------\n");        //due amount or the balance amount to be paid by the customer
              printf("Due Amount is                 : %d \n\n ",customer[i].due_amount);
              printf("-------------------------------------------------------------------\n\n"); 

              printf("**************************************************************************************\n");
              return;
        }
    }
    printf("\n\n Could not find the customer number. Please retry!\n");
}

void addAccounts()
{
       int s=0;
       customer[customerCount].number = customerCount + 1;
	       printf("***************************************************************\n");
	       printf("Customer no                     :%d\n",customer[customerCount].number);
            printf("Enter Customer Name             : ");
            scanf("%s[^\n]",customer[customerCount].name);
            printf("Payment Date (dd/mm/yyyy)       : ");
            scanf("%s[^\n]",customer[customerCount].date);
            printf("How many products %s has bought : ",customer[customerCount].name);
            scanf("%d",&customer[customerCount].no_of_products);
       for(int i=0 ; i<customer[customerCount].no_of_products ; i++)
       {
            printf("----------------------------------------\n");
            printf("Enter name of product %d       : ",i+1);
            scanf("%s[^\n]",customer[customerCount].product[i].item_name);
            printf("Enter unit price of product %d : ",i+1);
            scanf("%d",&customer[customerCount].product[i].unit_price);
            printf("Enter no of quantity purchased : ");
            scanf("%d",&customer[customerCount].product[i].no_of_items);
            //printf("----------------------------------------\n");
            customer[customerCount].product[i].total_price=((customer[customerCount].product[i].unit_price)*(customer[customerCount].product[i].no_of_items));
       }
       for(int i=0 ; i<customer[customerCount].no_of_products ; i++)
       {
          s = s + customer[customerCount].product[i].total_price;
       }
       customer[customerCount].bill_amount = s;
            printf("\nItem                      Unit Price       Quantity       Total Price \n");
       for(int i=0 ; i<customer[customerCount].no_of_products ; i++)
       {
            printf("   %s                           %d              %d               %d ",customer[customerCount].product[i].item_name,customer[customerCount].product[i].unit_price,customer[customerCount].product[i].no_of_items,customer[customerCount].product[i].total_price);
            printf("\n");      
       }
            printf("-------------------------------------------------------------------\n");
            printf("                    Total Bill Amount    :     %d         \n ",customer[customerCount].bill_amount);
            printf("-------------------------------------------------------------------\n");
            printf("Enter Paid Amount             : ");
            scanf("%d",&customer[customerCount].paid_amount);
            customer[customerCount].due_amount = customer[customerCount].bill_amount - customer[customerCount].paid_amount;
            printf("***************************************************************\n");          //due amount or the balance amount to be paid by the customer
            printf("Due Amount is                 : %d \n\n ",customer[customerCount].due_amount);
	       printf("***************************************************************\n");
     

    //customer[customerCount].id = customerCount;
    customerCount++;
}

int displayMenu()
{
    printf("\n*************** CUSTOMER BILLING SYSYTEM *************** \n");
    printf("........................................................ \n");
    printf("                          MENU                        \n");
    printf("...................................................... \n\n");
    printf("1. Add accounts. \n");
    printf("2. Search account by Customer Name. \n");
    printf("3. Search account by Customer Serial number. \n");
    printf("4. Display All customers. \n");
    printf("5. Exit Application. \n");
    printf("...................................................... \n\n");
    printf("Please Enter Your Choice ? : ");
    if(scanf("%d",&currentSelectedOption)!=1)
     {
          flush();
          return 0;
     }
     else
     {
          flush();
          return currentSelectedOption ;
     }
}

void loadSavedCustomers()
{
	FILE *infile; 
    	struct Customer tempCustomer;
    	infile = fopen ("accounts.txt", "r"); 

	if (infile == NULL)
		return;

	while(fread(&tempCustomer, sizeof(struct Customer), 1, infile)) 
	{
		customer[customerCount] = tempCustomer;
		customerCount++;
	}
}

void saveCustomers()
{
	FILE *fp;
	fp = fopen("accounts.txt","w");
	if(fp==NULL)
	{ 
        fprintf(stderr, "\nError opend file\n"); 
        exit (1); 
        } 

	for(int i=0 ; i<customerCount ; i++)
	{
		fwrite(&customer[i],sizeof(customer[i]),1,fp);
	}
	fclose(fp);
}
void flush()
{
     int c;
     while((c=getchar())!=EOF && c!='\n');
}
// END OF THE PROJECT.
