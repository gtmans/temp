// getDST Mans Rademaker 2022
//
// calculates start and end of European summertime for any year after 2021
// to set timeClient.setTimeOffset used by <NTPClient.h>
// European  summertime changes every last sunday of march and october

//NEW
String  yearStamp;
String  daysStamp;
int     Tyear;
int     daynr;
int     mon;
int     DOW;            // Day Of Week 0=sunday 6=saturday
int     offset=6;       // 202200101 -> 6
int     checkyear;
int     StartDST;
int     EndDST;
bool    LEAP=false;
bool    DEBUG=false;

String weekdays[7]  = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
String   months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};    
int   monthdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

//from old program TM1637v24-DST
String  formattedDate = "2022-04-03T16:00:13Z";//example
String  dayStamp;
String  monthStamp;
int     Tmonth;
int     Tday;
int     pos;
bool    DST;

void setup() {
  Serial.begin            (115200);
  while(!Serial);         // time to get serial running
  Serial.println          ();
  Serial.println          (__FILE__);//name of this doc
/*  
//RTC Initialize a NTPClient to get time
  timeClient.begin(); 
  timeClient.setTimeOffset(3600);  
  while(!timeClient.update()) {timeClient.forceUpdate();}
  // 2018-05-28T16:00:13Z
  formattedDate   = timeClient.getFormattedDate();
*/  

  Serial.println          (formattedDate);
  
  pos             = formattedDate.indexOf("T");
  dayStamp        = formattedDate.substring(0, pos);  //2022-05-28
  yearStamp       = dayStamp.substring(0,4);          //2022
  monthStamp      = dayStamp.substring(5,7);          //01
  daysStamp       = dayStamp.substring(8,10);         //01
  Tyear           = yearStamp.toInt();
  Tmonth          = monthStamp.toInt();
  Tday            = daysStamp.toInt();

  getDST(2022);
  /*
  if  (DST){
    timeClient.setTimeOffset(7200);  
    while(!timeClient.update()) {timeClient.forceUpdate();}
  }
  */
}
  
void loop() {
  // put your main code here, to run repeatedly:
}

// get the day of the week for 1st day of year
void getDOW(int d){
  int h = (d-1+offset)/7;
  DOW   =  d-1+offset - h*7;
 if (DEBUG){Serial.print(weekdays[DOW]);}
  mon = 0;
  while (d>monthdays[mon]){
    d=d-monthdays[mon];
    mon++;
  }
  daynr=d;
  if (DEBUG){
    Serial.print      (" ");
    Serial.print      (daynr);
    Serial.print      (" ");
    Serial.println    (months[mon]);    
  }
}

// check if year is a leap and set bool LEAP
void getLEAP(int y){ // leapyear div 4 or 400 but not 100 has 366 days io 365
  bool test1 = ((y-((y/400)*400))==0);  
  bool test2 = ((y-((y/100)*100))==0);  
  bool test3 = ((y-((y/4)*4))==0);  
  if (test1)    {LEAP=true;} 
  else 
    {if (test2) {LEAP=false;}
    else 
      {if (test3){LEAP=true;} 
      else {LEAP=false;} 
      }
    }
  if (LEAP){
    if (DEBUG){
      Serial.print      (y);
      Serial.print      (":");
      Serial.println    ("leapyear!");      
    }
    monthdays[1]=29;
  } else {
    monthdays[1]=28;}
}

// 01012022 sa 6 6 + 365 = 371 / 7 = 53 + 0
// 01012023 su 0 0 + 365 = 365 / 7 = 52 + 1
// 01012024 mo 1 1 + 366 = 367 / 7 = 52 + 3 leap but counts next year
// 01012025 we 3 3 + 365 = 368 / 7 = 52 + 4
void getDOWyear(int y){ // changes LEAP
  offset=6; // 202200101 = saturday = 6
  if (y==2022){DOW=offset;}
  else {
    for (checkyear = 2023; checkyear<=y;checkyear++){
      getLEAP(checkyear-1);
      if (LEAP){offset=offset+366;} else {offset=offset+365;}
      DOW   =  offset - 7*(offset/7);
    }  
  }  
  if (DEBUG){
    Serial.print      ("01-01-");
    Serial.print      (y);
    Serial.print      (" is een ");
    Serial.println    (weekdays[DOW]);
  }
  offset=DOW;// set first day of week for this year
}

void getDST(int t){
  Tyear=t;
  Serial.print      ("Finding  summertime period for ");
  Serial.println    (Tyear);
  getDOWyear        (Tyear);    // sets value of "offset" = day of week of 1/1/Tyear
  getLEAP           (Tyear);    // check if this year is a leap year sets bool LEAP


  StartDST=90+LEAP; // if bool LEAP == true then StartDST=91
  for (int test=90+LEAP; test>83+LEAP; test--) { // find last sunday in march
    getDOW(test);
    if (DOW==0){break;}
    StartDST--;
  }

//  if (DEBUG){
    Serial.print      ("Start    summertime is ");
    Serial.print      (weekdays[DOW]);
    Serial.print      (" ");
    Serial.print      (daynr);
    Serial.print      (" ");
    Serial.print      (months[mon]);
    Serial.print      (" day ");
    Serial.println    (StartDST);  
//  }

    EndDST=304+LEAP;
    for (int test=304+LEAP; test>287+LEAP; test--) { // find last sunday in october
    getDOW(test);
    if (DOW==0){break;}
    EndDST--;
}

//  if (DEBUG){
    Serial.print      ("End of   summertime is ");
    Serial.print      (weekdays[DOW]);
    Serial.print      (" ");
    Serial.print      (daynr);
    Serial.print      (" ");
    Serial.print      (months[mon]);
    Serial.print      (" day ");
    Serial.println    (EndDST);
//  }

/*}

void getDaynr(){
  getLEAP(Tyear);*/
  int today=0;
  for (mon=1; mon<Tmonth; mon++) { // get current day of year
    today+=monthdays[mon-1];
  }
  today=today+Tday;
  Serial.print      ("Today is day ");
  Serial.print      (today);
  Serial.print      (" of ");
  Serial.print      (Tyear);
  Serial.print      (" so it's ");
  if (today>=StartDST&&today<EndDST){DST=true;} else {DST=false;} 
  if (DST){Serial.println(" summertime!");} else {Serial.println("wintertime...");} 

}
