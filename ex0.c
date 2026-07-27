/*=========================================================
        NETWORK LAYERS SIMULATION
        PART 1
=========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*--------------- CONSTANTS ----------------*/

#define SIZE 10
#define MAXMESSAGE 1000
#define MAXBITS 10000
#define PACKETSIZE 16
#define FRAMESIZE 8

/*--------------- STRUCTURE ----------------*/

struct node
{
    char url[50];
    int ip[4];
    char mac[18];
    int flag;
};

struct node table[SIZE];

/*--------------- GLOBAL VARIABLES ----------------*/

char message[MAXMESSAGE];

char bitStream[MAXBITS];

char packets[500][PACKETSIZE + 1];

char frames[1000][FRAMESIZE + 1];

int totalBits = 0;
int totalPackets = 0;
int totalFrames = 0;
int sourcePort;
int destinationPort;

void networkLayer(int sourceIndex, int destinationIndex);
void dataLinkLayer(int sourceIndex, int destinationIndex);

/*=========================================================
                HASH FUNCTION
=========================================================*/

int hashFunction(char url[])
{
    int i;
    int sum = 0;

    for(i = 0; url[i] != '\0'; i++)
    {
        sum = sum + url[i];
    }

    return sum % SIZE;
}

/*=========================================================
                INSERT URL
=========================================================*/

void insertURL(char url[], int ip[], char mac[])
{
    int index;
    int i;

    index = hashFunction(url);

    for(i = 0; i < SIZE; i++)
    {
        if(table[index].flag == 0)
        {
            strcpy(table[index].url, url);

            table[index].ip[0] = ip[0];
            table[index].ip[1] = ip[1];
            table[index].ip[2] = ip[2];
            table[index].ip[3] = ip[3];

            strcpy(table[index].mac, mac);

            table[index].flag = 1;

            return;
        }

        index = (index + 1) % SIZE;
    }

    printf("\nHash Table Full.\n");
}

/*=========================================================
                SEARCH URL
=========================================================*/

int searchURL(char url[])
{
    int index;
    int i;

    index = hashFunction(url);

    for(i = 0; i < SIZE; i++)
    {
        if(table[index].flag == 1 &&
           strcmp(table[index].url, url) == 0)
        {
            return index;
        }

        index = (index + 1) % SIZE;
    }

    return -1;
}

/*=========================================================
                DELETE URL
=========================================================*/

void deleteURL(char url[])
{
    int pos;

    pos = searchURL(url);

    if(pos == -1)
    {
        printf("\nURL Not Found.\n");
        return;
    }

    table[pos].flag = 0;

    printf("\nDeleted Successfully.\n");
}

/*=========================================================
                DISPLAY TABLE
=========================================================*/

void displayTable()
{
    int i;

    printf("\n---------------------------------------------------------------\n");
    printf("Index\tURL\t\t\tIP Address\t\tMAC Address\n");
    printf("---------------------------------------------------------------\n");

    for(i = 0; i < SIZE; i++)
    {
        if(table[i].flag == 1)
        {
            printf("%d\t", i);

            printf("%-20s", table[i].url);

            printf("%d.%d.%d.%d\t",
                   table[i].ip[0],
                   table[i].ip[1],
                   table[i].ip[2],
                   table[i].ip[3]);

            printf("%s\n", table[i].mac);
        }
        else
        {
            printf("%d\tEMPTY\n", i);
        }
    }
}


/*=========================================================
            LOAD DEFAULT URLS
=========================================================*/

void loadDefaultURLs()
{
    int ip1[4] = {142,250,10,1};
    int ip2[4] = {142,250,10,2};
    int ip3[4] = {157,240,1,35};
    int ip4[4] = {205,251,242,103};
    int ip5[4] = {208,80,154,224};

    insertURL("www.google.com",ip1,"AA:11:22:33:44:55");

    insertURL("www.youtube.com",ip2,"AA:11:22:33:44:66");

    insertURL("www.facebook.com",ip3,"BB:22:33:44:55:66");

    insertURL("www.amazon.com",ip4,"CC:11:22:33:44:55");

    insertURL("www.wikipedia.org",ip5,"DD:11:22:33:44:55");
}

/*=========================================================
                DECIMAL TO BINARY
=========================================================*/

void decimalToBinary(int number, int bits, char binary[])
{
    int i;

    binary[bits] = '\0';

    for(i = bits - 1; i >= 0; i--)
    {
        binary[i] = (number % 2) + '0';
        number = number / 2;
    }
}

/*=========================================================
                CHARACTER TO BINARY
=========================================================*/

void characterToBinary(char ch, char binary[])
{
    decimalToBinary((unsigned char)ch, 8, binary);
}

/*=========================================================
                READ MESSAGE
=========================================================*/

void readMessage(char fileName[])
{
    FILE *fp;
    int i = 0;
    char ch;

    fp = fopen(fileName, "r");

    if(fp == NULL)
    {
        printf("\nCannot Open File.\n");
        return;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        message[i] = ch;
        i++;
    }

    message[i] = '\0';

    fclose(fp);
}

/*=========================================================
                MESSAGE TO BINARY
=========================================================*/

void messageToBinary()
{
    int i;
    char binary[9];

    bitStream[0] = '\0';

    printf("\n=========== APPLICATION LAYER ===========\n");

    printf("\nOriginal Message : %s\n", message);

    printf("\nCharacter to Binary\n\n");



    for(i = 0; message[i] != '\0'; i++)
    {
        characterToBinary(message[i], binary);

        printf("%c  -->  %s\n", message[i], binary);

        strcat(bitStream, binary);
    }

    totalBits = strlen(bitStream);

    printf("\nComplete Bit Stream\n");

    printf("%s\n", bitStream);

    printf("\nTotal Bits = %d\n", totalBits);
}

/*=========================================================
                DISPLAY MESSAGE
=========================================================*/

void displayMessage()
{
    printf("\nMessage : %s\n", message);
}


/*=========================================================
                HEX TO DECIMAL
=========================================================*/

int hexToDecimal(char ch)
{
    if(ch >= '0' && ch <= '9')
        return ch - '0';

    if(ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;

    if(ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    return 0;
}

/*=========================================================
                IP TO BINARY
=========================================================*/

void ipToBinary(int ip[], char binary[])
{
    int i;
    char temp[9];

    binary[0] = '\0';

    for(i = 0; i < 4; i++)
    {
        decimalToBinary(ip[i], 8, temp);
        strcat(binary, temp);
    }
}

/*=========================================================
                MAC TO BINARY
=========================================================*/

void macToBinary(char mac[], char binary[])
{
    int i;
    int value;
    char temp[9];

    binary[0]='\0';

    for(i = 0; mac[i] != '\0'; i++)
    {
        if(mac[i] == ':')
            continue;

        value = hexToDecimal(mac[i]) * 16;
        value = value + hexToDecimal(mac[i+1]);

        decimalToBinary(value,8,temp);

        strcat(binary,temp);

        i++;
    }
}

/*=========================================================
                PORT TO BINARY
=========================================================*/

void portToBinary(int port,char binary[])
{
    decimalToBinary(port,16,binary);
}

/*=========================================================
                DISPLAY IP
=========================================================*/

void displayIP(int ip[])
{
    printf("%d.%d.%d.%d",
           ip[0],
           ip[1],
           ip[2],
           ip[3]);
}

/*=========================================================
                DISPLAY MAC
=========================================================*/

void displayMAC(char mac[])
{
    printf("%s", mac);
}

/*=========================================================
                APPLICATION LAYER
=========================================================*/

void applicationLayer()
{
    readMessage("message.txt");

    messageToBinary();
}
/*=========================================================
                TRANSPORT LAYER
=========================================================*/

void transportLayer()
{
    

    char sourcePortBinary[17];
    char destinationPortBinary[17];

    printf("\n========================================");
    printf("\n         TRANSPORT LAYER");
    printf("\n========================================\n");

    sourcePort = rand() % (65535 - 1024 + 1) + 1024;
    destinationPort = rand() % (65535 - 1024 + 1) + 1024;

    portToBinary(sourcePort, sourcePortBinary);
    portToBinary(destinationPort, destinationPortBinary);

    printf("\nSource Port      : %d", sourcePort);
    printf("\nDestination Port : %d", destinationPort);

    printf("\n\nSource Port Binary      : %s", sourcePortBinary);
    printf("\nDestination Port Binary : %s\n", destinationPortBinary);
}
/*=========================================================

START TRANSMISSION
=========================================================*/

void startTransmission()
{
    char sourceURL[50];
    char destinationURL[50];

    int sourceIndex;
    int destinationIndex;

    int ip[4];
    char mac[18];

    printf("\nEnter Source URL : ");
    scanf("%s", sourceURL);

    printf("Enter Destination URL : ");
    scanf("%s", destinationURL);

    /*---------------- SOURCE ----------------*/

    sourceIndex = searchURL(sourceURL);

    if(sourceIndex == -1)
{
    printf("\nSource URL Not Found.\n");

    printf("Enter Source IP Address: ");
    scanf("%d %d %d %d",
          &ip[0], &ip[1], &ip[2], &ip[3]);

    printf("Enter Source MAC Address: ");
    scanf("%s", mac);

    insertURL(sourceURL, ip, mac);

    sourceIndex = searchURL(sourceURL);
}

    /*---------------- DESTINATION ----------------*/

    destinationIndex = searchURL(destinationURL);
    if(destinationIndex == -1)
    {

        printf("\nDestination URL Not Found.\n");

        printf("Enter Destination IP Address: ");
        scanf("%d %d %d %d",
              &ip[0], &ip[1], &ip[2], &ip[3]);

        printf("Enter Destination MAC Address: ");
        scanf("%s", mac);

        insertURL(destinationURL, ip, mac);

        destinationIndex = searchURL(destinationURL);
    }
        printf("\n========================================");
        printf("\nSOURCE DETAILS");
        printf("\n========================================\n");

        printf("URL : %s\n", table[sourceIndex].url);

        printf("IP  : ");
        displayIP(table[sourceIndex].ip);

        printf("\nMAC : ");
        displayMAC(table[sourceIndex].mac);

        printf("\n");
        printf("\n========================================");
        printf("\nDESTINATION DETAILS");
        printf("\n========================================\n");

        printf("URL : %s\n", table[destinationIndex].url);

        printf("IP  : ");
        displayIP(table[destinationIndex].ip);

        printf("\nMAC : ");
        displayMAC(table[destinationIndex].mac);

        printf("\n");

        applicationLayer();

    transportLayer();

    networkLayer(sourceIndex, destinationIndex);

    dataLinkLayer(sourceIndex, destinationIndex);
}
/*=========================================================
                NETWORK LAYER
=========================================================*/

void networkLayer(int sourceIndex, int destinationIndex)
{
    int i;
    int j;

    char sourceIPBinary[33];
    char destinationIPBinary[33];

    ipToBinary(table[sourceIndex].ip, sourceIPBinary);
    ipToBinary(table[destinationIndex].ip, destinationIPBinary);

    totalPackets = 0;

    printf("\n========================================");
    printf("\n          NETWORK LAYER");
    printf("\n========================================\n");

    for(i = 0; i < totalBits; i += PACKETSIZE)
    {
        for(j = 0; j < PACKETSIZE; j++)
        {
            if(i + j < totalBits)
                packets[totalPackets][j] = bitStream[i + j];
            else
                packets[totalPackets][j] = '0';
        }

        packets[totalPackets][PACKETSIZE] = '\0';

        printf("\nPacket %d\n", totalPackets + 1);

        printf("Source IP Binary      : %s\n",
               sourceIPBinary);

        printf("Destination IP Binary : %s\n",
               destinationIPBinary);

        printf("Packet Data           : %s\n",
               packets[totalPackets]);

        totalPackets++;
    }

    printf("\nTotal Packets = %d\n",
           totalPackets);
}
/*=========================================================
                DATA LINK LAYER
=========================================================*/
void dataLinkLayer(int sourceIndex,
                   int destinationIndex)
{
    int i;
    int frameNo = 0;

    char sourceMACBinary[49];
    char destinationMACBinary[49];

    char trailer[] = "00000000";

    char sourceIPBinary[33];
    char destinationIPBinary[33];

    char sourcePortBinary[17];
    char destinationPortBinary[17];

    char completeFrame[300];

    macToBinary(table[sourceIndex].mac,
                sourceMACBinary);

    macToBinary(table[destinationIndex].mac,
                destinationMACBinary);

    ipToBinary(table[sourceIndex].ip,
           sourceIPBinary);

    ipToBinary(table[destinationIndex].ip,
           destinationIPBinary);

    portToBinary(sourcePort,
             sourcePortBinary);

    portToBinary(destinationPort,
             destinationPortBinary);

    totalFrames = 0;

    printf("\n========================================");
    printf("\n         DATA LINK LAYER");
    printf("\n========================================\n");

    for(i = 0; i < totalPackets; i++)
    {
        strncpy(frames[frameNo],
                packets[i],
                8);

        frames[frameNo][8] = '\0';

        printf("\nFrame %d\n",
               frameNo + 1);

        printf("Source MAC Binary      : %s\n",
               sourceMACBinary);

        printf("Destination MAC Binary : %s\n",
               destinationMACBinary);

        printf("Data                  : %s\n",
               frames[frameNo]);

        printf("Trailer               : %s\n",
               trailer);

        frameNo++;
        totalFrames++;

        strncpy(frames[frameNo],
                packets[i] + 8,
                8);

        frames[frameNo][8] = '\0';

        printf("\nFrame %d\n",
               frameNo + 1);

        printf("Source MAC Binary      : %s\n",
               sourceMACBinary);


               printf("Destination MAC Binary : %s\n",
               destinationMACBinary);

        printf("Data                  : %s\n",
               frames[frameNo]);


        printf("Trailer               : %s\n",
               trailer);

        frameNo++;
        totalFrames++;
    }
    printf("\n========================================");
    printf("\n COMPLETE FRAME TRANSMISSION");
    printf("\n========================================\n");

    for(i = 0; i < totalFrames; i++)
    {
        completeFrame[0] = '\0';

        strcat(completeFrame, sourceMACBinary);
        strcat(completeFrame, destinationMACBinary);

        strcat(completeFrame, sourceIPBinary);
        strcat(completeFrame, destinationIPBinary);

        strcat(completeFrame, sourcePortBinary);
        strcat(completeFrame, destinationPortBinary);

        strcat(completeFrame, frames[i]);

        strcat(completeFrame, trailer);

        printf("\nComplete Frame %d\n", i + 1);
        printf("%s\n", completeFrame);
    }

    printf("\nTotal Frames = %d\n",
           totalFrames);
}
/*=========================================================
                    SUMMARY
=========================================================*/

void summary()
{
    printf("\n========================================");
    printf("\n             SUMMARY");
    printf("\n========================================");

    printf("\nTotal Bits    : %d", totalBits);
    printf("\nTotal Packets : %d", totalPackets);
    printf("\nTotal Frames  : %d", totalFrames);

    printf("\n========================================\n");
}
/*=========================================================
                    MAIN FUNCTION
=========================================================*/

int main()
{
    int choice;

    char url[50];

    int ip[4];

    char mac[18];

    loadDefaultURLs();

    while(1)
    {
        printf("\n========================================");
        printf("\n      NETWORK LAYER SIMULATION");
        printf("\n========================================");

        printf("\n1. Insert URL");
        printf("\n2. Delete URL");
        printf("\n3. Search URL");
        printf("\n4. Display Hash Table");
        printf("\n5. Start Transmission");
        printf("\n6. Exit");

        printf("\n\nEnter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
        case 1:

            printf("\nEnter URL : ");
            scanf("%s", url);

            printf("Enter IP Address (Example: 142 250 10 1): ");
            scanf("%d %d %d %d",&ip[0], &ip[1], &ip[2], &ip[3]);

            printf("Enter MAC Address (Example: AA:11:22:33:44:55): ");
            scanf("%s", mac);

            insertURL(url, ip, mac);

            printf("\nInserted Successfully.\n");
            break;
        case 2:

            printf("\nEnter URL : ");
            scanf("%s",url);

            deleteURL(url);

            break;

        case 3:
        {
            int pos;

            printf("\nEnter URL : ");
            scanf("%s",url);

            pos = searchURL(url);

            if(pos==-1)
            {
                printf("\nURL Not Found.\n");
            }
            else
            {
                printf("\nURL Found.\n");

                printf("\nURL : %s",table[pos].url);

                printf("\nIP : ");
                displayIP(table[pos].ip);

                printf("\nMAC : %s\n",table[pos].mac);
            }

            break;
        }

        case 4:

            displayTable();

            break;
        
        case 5:


            startTransmission();

            summary();

            break;

        case 6:

            printf("\nProgram Ended.\n");

            exit(0);

        default:

            printf("\nInvalid Choice.\n");

        }

    }

    return 0;
}