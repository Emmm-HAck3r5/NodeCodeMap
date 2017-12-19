typedef enum Day{
    SUM = 7,
    MON = 1,
    TUE = 2,
    WED = 3,
    THU = 4,
    FRI = 5,
    SAT = 6
}Day;

typedef struct OWNER {
    Day day;
}OWNER;

extern OWNER* returnEnum();