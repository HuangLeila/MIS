#include"Util.h"

string Util::INPUTPATH = "C:\\Users\\黄乐\\Desktop\\ColdRollSchedule_testCase\\ColdRollSchedule\\Input\\"; //"E:\\BG2\\ColdRollSchedule\\Input\\Test";
string Util::OUTPUTPATH = "C:\\Users\\黄乐\\Desktop\\ColdRollSchedule_testCase\\ColdRollSchedule\\Output\\"; //"E:\\BG2\\ColdRollSchedule\\Output\\";
time_t Util::bgnTime = makeTime(2018, 10, 8);
/*终止日期第二天一早上*/
time_t Util::endTime = makeTime(2018, 10, 15);