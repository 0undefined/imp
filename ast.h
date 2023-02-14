typedef enum {
  AExpr_num,
  AExpr_loc,

  AExpr_add,
  AExpr_sub,
  AExpr_mul,
  AExpr_div,
} AExpr_t;

typedef enum {
  i64, i32, i16, i8,
  u64, u32, u16, u8,
  f64, f32, f16, f8,
  bool,
} type_t;

typedef struct {
  AExpr_t type;
  union {
    int   num;
    char* loc;
    struct {struct AExpr *l; struct AExpr *r;} add;
    struct {struct AExpr *l; struct AExpr *r;} sub;
    struct {struct AExpr *l; struct AExpr *r;} mul;
  } exp;
} AExpr;

AExpr new_num(int num) {
  return (AExpr){
    .type = AExpr_num,
    .exp = {.num = num}
  };
}
