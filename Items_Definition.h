#ifndef ITEMS_DEFINITION_H
#define ITEMS_DEFINITION_H

#include <QDate>

#define DB_PR_LIST      "PR_Tracker"
#define DB_PR_ITEMS     "PR_Items"
#define DB_DEPARTMENTS  "Departments"
#define DB_PR_LIST_NEW  "PR_Number"
#define DB_PR_PARTS     "PR_Parts"
#define DB_ITEMS        "Items"



#define PR_ID           "ID"
#define PR_NUM          "PR_Number"
#define PR_SUBMIT_DATE  "Submit_Date"
#define PR_DEPARTMENT   "Department"
#define PR_PURPOSE      "Purpose"
#define PR_ITEM_COUNT   "Itemes_Count"
#define PR_STATUS       "Status"
#define PR_REMARKS      "Remark"
#define PR_TAG          "TAG"
#define PR_SUBJECT      "Subjects"


#define ITEM_ID         "ID"
#define ITEM_PR_NUM     "PR_Number"
#define ITEM_CODE       "Item_Code"
#define ITEM_DESC       "Description"
#define ITEM_UOM        "UOM"
#define ITEM_QUANTITY   "Quantity"
#define ITEM_PROGRESS   "Progress"
#define ITEM_STATUS     "Status"
#define ITEM_REMARKS    "Remarks"
#define ITEM_CLASS      "Class"

#define PART_ID         "ID"
#define PART_CODE       "Code"
#define PART_DESCRIP    "Description"
#define PART_UOM        "UOM"
#define PART_MINI_COUNT "MCount"
#define PART_TYPE       "Type"




typedef struct
{
    int ID;
    int PR_Number;
    QDate Submit_Date;
    QString Department;
    QString Purpose;
    int Item_Count;
    QString Status;
    QString Remarks;
    QString TAG;
    QString Subjects;

}PR_Number;


typedef struct
{
    int ID;
    int PR_Number;
    QString Item_Code;
    QString Description;
    QString UOM;
    int Quantity;
    int Progress;
    QString Status;
    QString Remarks;
    QString Classification;
}PR_Items;

#endif // ITEMS_DEFINITION_H
