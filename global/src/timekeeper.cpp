#include <iostream>
#include <fstream>
#include <ctime>
int main( )
{
   time_t now = time(0);
   tm* loc_time = localtime(&now);
   char buffer[80];
   strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S +0000", loc_time);

   std::ofstream timestampFile;
   timestampFile.open ("build_timestamp");
   timestampFile << buffer;
   timestampFile.close();
   return 0;
}
