/******************************************************************************
 *                                  TODO
******************************************************************************/
// DONE: FUNC_WRITE

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"
#include "str.h"
#include "queue.h"
#include "symtable.h"


/******************************************************************************
  *									MACROS
*****************************************************************************/
#define BLOCKS_NUM 2

#define DEBUG_ANDREJ 0
#define DEBUG_ZDENEK 1

#define DEBUG_INSTR         1
#define DEBUG_INSTR_2       1

#define IFJ_CODE_START_LEN 10000
#define MAX_LINE_LEN       300
#define EOL                 "\n"
#define EMPTY_STR           ""
#define NON_VAR             "%s"
#define DEVIDER             "################################################################################# "
#define DEVIDER_2           "########## "
#define FORMAT_VAR          " LF@$%s$%lu$%s$ "
#define FORMAT_PARAM        " LF@%%%dp "
#define FORMAT_IF           " $%s$%d$if$ "
#define FORMAT_ELSE         " $%s$%d$else$ "
#define FORMAT_IF_END       " $%s$%d$if_end$ "


#define INIT_CONCAT_STR(num, fmt, ...)                                                      \
    do {                                                                                    \
        if (!DEBUG_INSTR_2) {                                                               \
            sprintf(instr##num, (fmt EOL), __VA_ARGS__);                                    \
        } else {                                                                            \
            snprintf(instr##num, MAX_LINE_LEN, (fmt "%*s:%d:%s():" EOL), __VA_ARGS__,       \
                                    80-snprintf(NULL, 0, (fmt), __VA_ARGS__),           \
                                    "#", __LINE__, __func__);                               \
        }                                                                                   \
    } while(0)                                                                              \

#define PRINT_MAIN(num, fmt, ...)                                                           \
    do {                                                                                    \
        char instr##num[(snprintf(NULL, 0, (fmt), __VA_ARGS__) + MAX_LINE_LEN)];            \
        INIT_CONCAT_STR(num, fmt, __VA_ARGS__);                                             \
        if (!str_concat_str2(&ifj_code[MAIN], instr##num)) {                                \
            return false;                                                                   \
        }                                                                                   \
    } while(0)

#define PRINT_FUNC(num, fmt, ...)                                                           \
    do {                                                                                    \
        char instr##num[(snprintf(NULL, 0, (fmt), __VA_ARGS__) + MAX_LINE_LEN)];            \
        INIT_CONCAT_STR(num, fmt, __VA_ARGS__);                                                                                    \
        if (!str_concat_str2(&ifj_code[FUNCTIONS], instr##num)) {                           \
            return false;                                                                   \
        }                                                                                   \
    } while(0)

#define PRINT_FUNC_BUILT_IN(num, fmt, ...)                                                           \
    do {                                                                                    \
        char instr##num[(snprintf(NULL, 0, (fmt), __VA_ARGS__) + MAX_LINE_LEN)];            \
        sprintf(instr##num, (fmt EOL), __VA_ARGS__);                                    \
        if (!str_concat_str2(&ifj_code[FUNCTIONS], instr##num)) {                           \
            return false;                                                                   \
        }                                                                                   \
    } while(0)



#if DEBUG_INSTR
#define DEBUG_PRINT_INSTR(num, NUM_BLOCK ,fmt, ...)                                 \
		do {                                                                        \
			char instr##num[(snprintf(NULL, 0, (fmt), __VA_ARGS__) + MAX_LINE_LEN)];           \
			INIT_CONCAT_STR(num, fmt, __VA_ARGS__);                                         \
			if (!str_concat_str2(&ifj_code[NUM_BLOCK], instr##num)) {               \
				return false;                                                       \
			}                                                                       \
		} while(0)
#else
#define DEBUG_PRINT_INSTR(num, fmt, ...)
#endif // DEBUG_INSTR

#define PRINT_WHERE(...)                        \
    do {                                        \
        if (strcmp(cnt.func_name.str, "") == 0) {   \
                PRINT_MAIN(__VA_ARGS__);            \
        } else {                                    \
                PRINT_FUNC(__VA_ARGS__);            \
        }                                           \
    } while(0)                                  \


#define SWITCH_CASE(number)                             \
        case number:                                    \
            sprintf(tmp_str, "%d", number);             \
            if(!str_add_char(&str_out, '\\')){          \
                return false;                            \
            }                                           \
            if(!str_add_char(&str_out, '0')){           \
                return false;                            \
            }                                           \
            if(!str_add_char(&str_out, tmp_str[0])){    \
                return false;                             \
            }                                           \
            if (tmp_str[1] != '\0'){                    \
                if(!str_add_char(&str_out, tmp_str[1])){      \
                    return false;                         \
                }                                       \
            }                                           \
            break;

#define CODE_GEN(callback, ...)         \
    do {                                \
        if (!(callback)(__VA_ARGS__)) {   \
            err = INTERNAL_ERR;                                     \
            return false;                                           \
        }                                                           \
    } while(0)                          \

#define QUEUE_ADD_ID(where_is_id_key)                               \
    do {                                                            \
        if (strcmp(cnt.func_call.str, "write") == 0) {              \
               break;                                               \
        } else if (!queue_add_id_rear(queue_id, (where_is_id_key))) {    \
            err = INTERNAL_ERR;                                     \
            return false;                                           \
        }                                                           \
    } while(0);                                                     \

#define QUEUE_ADD_ARGS(where_is_id_key) \
    do {                              \
        if (!queue_add_id_rear(queue_args, (where_is_id_key))) {    \
            err = INTERNAL_ERR;                             \
            return false;                                   \
        }                           \
    } while(0);                                 \


/******************************************************************************
  *									STRUCTS
*****************************************************************************/

typedef struct cnts_s {
    string_t func_name;
    string_t func_call;
    unsigned int param_cnt;
    unsigned int if_cnt;
    unsigned int while_cnt;
    unsigned int deep;
    unsigned int ret_vals;
} cnts_t;

extern cnts_t cnt;
extern string_t ifj_code[BLOCKS_NUM];

enum block_e {FUNCTIONS, MAIN};

typedef long long unsigned int llu_t;

/******************************************************************************
  *									FUNCTIONS
*****************************************************************************/


bool alloc_ifj_code();
bool init_ifj_code();

bool alloc_cnt();
bool init_cnt();

bool gen_init_built_int();

bool gen_while_label();
bool gen_while_start();
bool gen_while_end();
bool gen_while_eval();

bool gen_params();
bool gen_param();

bool gen_if_start();
bool gen_if_else(/*TODO*/);
bool gen_if_end();

bool gen_func_start(char *id);
bool gen_func_end();
bool gen_func_call_write();
bool gen_func_call_start();
bool gen_func_call_args_var();
bool gen_func_call_args_const();
bool gen_func_call_label();

bool code_gen_print_ifj_code21();
bool gen_expression();
bool gen_init();
bool code_gen();
bool gen_expression();
bool gen_if_eval();
bool gen_if_end_jump();
bool gen_def_var();
bool gen_init_var();
bool dealloc_gen_var();
bool gen_retval_nil();
bool gen_testing_helper();

int where_to_print();
bool is_write();


/******************************************************************************
  *									BUILT-IN FUNCS
*****************************************************************************/
#define FUNC_TOINTEGER                                                        \
"label $tointeger # tointeger(f : number) : integer						"\
"\n	# start																	"\
"\n	createframe			# new TF											"\
"\n	pushframe			# TF => LF											"\
"\n																			"\
"\n	# logic																	"\
"\n	defvar		LF@ret1														"\
"\n	defvar		LF@$tointeger_var_type										"\
"\n																			"\
"\n	pops 		LF@ret1														"\
"\n	TYPE		LF@$tointeger_var_type LF@ret1								"\
"\n																			"\
"\n	jumpifneq 	$tointeger_end LF@$tointeger_var_type string@float			"\
"\n																			"\
"\n	label t_float															"\
"\n	float2int 	LF@ret1 	LF@ret1											"\
"\n	jump 		$tointeger_end												"\
"\n																			"\
"\n	## end																	"\
"\n	label $tointeger_end													"\
"\n	pushs LF@ret1															"\
"\n	popframe 			# LF => TF											"\
"\n	return																	"

#define FUNC_READI                                                        \
"label $readi # readi() : integer"\
"\ncreateframe"\
"\npushframe"\
"\ndefvar LF@readi_ret1"\
"\nread   LF@readi_ret1 int"\
"\npushs  LF@readi_ret1"\
"\npopframe"\
"\nreturn"

#define FUNC_READN              \
"label $readn # readn() : number"  \
"\ncreateframe"            \
"\npushframe"              \
"\ndefvar LF@readn_ret1"   \
"\nread   LF@readn_ret1 float"\
"\npushs  LF@readn_ret1"   \
"\npopframe"               \
"\nreturn"

#define FUNC_READS \
"label $reads # reads() : string	"\
"\ncreateframe"            \
"\npushframe"              \
"\ndefvar 	LF@reads_ret1"  \
"\nread    LF@reads_ret1 string"\
"\npushs   LF@reads_ret1"  \
"\npopframe"               \
"\nreturn"

#define FUNC_WRITE      \
"label &write"        \
"\npushframe"           \
"\ncreateframe"         \
"\ndefvar 		LF@&write_var_type"                                 \
"\ntype		    LF@&write_var_type 	LF@%0p"                         \
"\njumpifeq 	$write$type$nil 	LF@&write_var_type string@nil"  \
"\nwrite 		LF@%0p"                 \
"\njump 		$write$end"             \
"\nlabel 		$write$type$nil"        \
"\nwrite 		string@nil"             \
"\nlabel $write$end"                    \
"\npopframe"                            \
"\nreturn"


#if 0
"\nlabel $write # write(... : string | integer | number | boolean)  -- podpora boolean pro bonusove rozsireni	"\
"\n	# start																										"\
"\n	createframe																									"\
"\n	pushframe																									"\
"\n																												"\
"\n	# logic																										"\
"\n	defvar 		LF@write_var_type																				"\
"\n	defvar 		LF@write_var_print																				"\
"\n																												"\
"\n	label $write_while1																							"\
"\n	pops 		LF@write_var_print																				"\
"\n	TYPE		LF@write_var_type 	LF@write_var_print 															"\
"\n	jumpifeq 	$write_label_t_nil 	LF@write_var_type string@nil												"\
"\n	jumpifeq 	$write_label_string	LF@write_var_type string@string												"\
"\n	write 		LF@write_var_print																				"\
"\n	jump 		$write_while1																					"\
"\n																												"\
"\n	label $write_label_string																					"\
"\n	jumpifeq 	$write_label_end	LF@write_var_print GF@bottom_of_stack										"\
"\n	write 		LF@write_var_print																				"\
"\n	jump 		$write_while1																					"\
"\n																												"\
"\n	label 		$write_label_t_nil																				"\
"\n	write 		string@nil																						"\
"\n	jump 		$write_while1																					"\
"\n																												"\
"\n	# end																										"\
"\n	label $write_label_end																						"\
"\n	popframe																									"\
"\n	return																										"
#endif


#define FUNC_SUBSTR                                                        \
"\nlabel $substr # substr(str : string, i : integer, j : integer) : string 	"\
"\n	# start																	"\
"\n	createframe																"\
"\n	pushframe																"\
"\n																			"\
"\n	# logic																	"\
"\n	defvar 	LF@substr_param1												"\
"\n	defvar 	LF@substr_param2												"\
"\n	defvar 	LF@substr_param3												"\
"\n	defvar 	LF@substr_ret1													"\
"\n	defvar 	LF@substr_len													"\
"\n	defvar 	LF@substr_while_cnt												"\
"\n	defvar 	LF@substr_char													"\
"\n	defvar 	LF@substr_cmp													"\
"\n																			"\
"\n	pops 	LF@substr_param1												"\
"\n	pops 	LF@substr_param2												"\
"\n	pops 	LF@substr_param3												"\
"\n	move 	LF@substr_ret1 		string@										"\
"\n	strlen 	LF@substr_len 		LF@substr_param1							"\
"\n	move 	LF@substr_while_cnt	int@0										"\
"\n	move 	LF@substr_char 		string@										"\
"\n	move 	LF@substr_cmp 		bool@false									"\
"\n																			"\
"\n	# j > i																	"\
"\n	gt 			LF@substr_cmp 		LF@substr_param2 	LF@substr_param3	"\
"\n	JUMPIFEQ 	$substr_label_end2 	LF@substr_cmp 		bool@true			"\
"\n	# i < 0																	"\
"\n	lt 			LF@substr_cmp 		LF@substr_param2 	int@0				"\
"\n	JUMPIFEQ 	$substr_label_end2 	LF@substr_cmp 		bool@true			"\
"\n	# j < 0																	"\
"\n	lt 			LF@substr_cmp 		LF@substr_param3 	int@0				"\
"\n	JUMPIFEQ 	$substr_label_end2 	LF@substr_cmp 		bool@true			"\
"\n	# i > len-1																"\
"\n	lt 			LF@substr_cmp 		LF@substr_param2 	LF@substr_len		"\
"\n	JUMPIFNEQ 	$substr_label_end2 	LF@substr_cmp 		bool@true			"\
"\n	# j > len-1																"\
"\n	lt 			LF@substr_cmp 		LF@substr_param3 	LF@substr_len		"\
"\n	JUMPIFNEQ 	$substr_label_end2 	LF@substr_cmp 		bool@true			"\
"\n																			"\
"\n																			"\
"\n	label $substr_while1													"\
"\n	getchar  LF@substr_char 		LF@substr_param1 	LF@substr_param2	"\
"\n	concat   LF@substr_ret1 		LF@substr_ret1		LF@substr_char		"\
"\n	jumpifeq $substr_label_end 		LF@substr_param2 	LF@substr_param3	"\
"\n	add 	 LF@substr_while_cnt	LF@substr_while_cnt int@1				"\
"\n	add 	 LF@substr_param2 		LF@substr_param2 	int@1				"\
"\n	jump 	 $substr_while1													"\
"\n																			"\
"\n	# end																	"\
"\n	label $substr_label_end													"\
"\n	clears																	"\
"\n	pushs 	GF@bottom_of_stack												"\
"\n	pushs 	LF@substr_ret1													"\
"\n	jump 	$substr_label_end3												"\
"\n																			"\
"\n	# end 2																	"\
"\n	label $substr_label_end2												"\
"\n	clears																	"\
"\n	pushs 	GF@bottom_of_stack												"\
"\n	pushs 	nil@nil															"\
"\n																			"\
"\n	# end 3																	"\
"\n	label $substr_label_end3												"\
"\n	popframe																"\
"\n	return																	"

#define FUNC_ORD                                                        \
"\nlabel $ord # ord(s : string, i : integer) : integer					"\
"\n	# start																"\
"\n	createframe															"\
"\n	pushframe															"\
"\n																		"\
"\n	# logic																"\
"\n	defvar 		LF@ord_param1											"\
"\n	defvar 		LF@ord_param2											"\
"\n	defvar 		LF@ord_cmp												"\
"\n	defvar 		LF@ord_ret1												"\
"\n	defvar 		LF@ord_len												"\
"\n																		"\
"\n	pops 		LF@ord_param1											"\
"\n	pops 		LF@ord_param2											"\
"\n	move 		LF@ord_ret1 	bool@false								"\
"\n	move 		LF@ord_ret1 	nil@nil									"\
"\n	strlen		LF@ord_len 		LF@ord_param1							"\
"\n																		"\
"\n	gt 			LF@ord_cmp 		LF@ord_param2 	int@255					"\
"\n	JUMPIFEQ 	$ord_label_end 	LF@ord_cmp 		bool@true				"\
"\n	lt 			LF@ord_cmp 		LF@ord_param2 	int@0					"\
"\n	JUMPIFEQ 	$ord_label_end 	LF@ord_cmp 		bool@true				"\
"\n	lt 			LF@ord_cmp 		LF@ord_param2	LF@ord_len				"\
"\n	JUMPIFNEQ 	$ord_label_end 	LF@ord_cmp 		bool@true				"\
"\n																		"\
"\n	stri2int 	LF@ord_ret1 LF@ord_param1 LF@ord_param2					"\
"\n																		"\
"\n	label $ord_label_end												"\
"\n	pushs 		LF@ord_ret1												"\
"\n																		"\
"\n	# end																"\
"\n	popframe															"\
"\n	return																"

#define FUNC_CHR                                                        \
"\nlabel $chr # chr(i : integer) : string								"\
"\n	## start															"\
"\n	createframe															"\
"\n	pushframe															"\
"\n																		"\
"\n	# logic																"\
"\n	defvar 		LF@chr_param1											"\
"\n	defvar 		LF@chr_ret1												"\
"\n	defvar 		LF@chr_cmp												"\
"\n																		"\
"\n	pops 		LF@chr_param1											"\
"\n	move 		LF@chr_ret1 	nil@nil									"\
"\n																		"\
"\n	gt 			LF@chr_cmp 		LF@chr_param1 	int@255					"\
"\n	JUMPIFEQ 	$chr_label_end 	LF@chr_cmp 		bool@true				"\
"\n	lt 			LF@chr_cmp 		LF@chr_param1 	int@0					"\
"\n	JUMPIFEQ 	$chr_label_end 	LF@chr_cmp 		bool@true				"\
"\n																		"\
"\n	int2char 	LF@chr_ret1 	LF@chr_param1							"\
"\n																		"\
"\n	## end																"\
"\n	label $chr_label_end												"\
"\n	pushs 		LF@chr_ret1												"\
"\n	popframe															"\
"\n	return																"                                             \


/******************************************************************************
  *									AUXILIARY FUNCS
*****************************************************************************/



#define FUNC_OP_NIL     \
"\nlabel $op_nil"       \
"\nexit int@8\n"

#define FUNC_RETYPING_VAR1      \
"\nlabel $retyping_var1"        \
"\nint2float GF@&var1 GF@&var1" \
"\nreturn\n"

#define FUNC_RETYPING_VAR2          \
"\nlabel $retyping_var2"            \
"\nint2float GF@&var2 GF@&var2"     \
"\nreturn\n"

#define FUNC_CHECK_OP                                         \
"\nlabel $check_op"                                           \
"\npops GF@&var2"                                             \
"\npops GF@&var1"                                             \
"\ntype GF@&type1 GF@&var1"                                   \
"\ntype GF@&type2 GF@&var2"                                   \
"\njumpifeq $op_nil GF@&type1 string@nil"                     \
"\njumpifeq $op_nil GF@&type2 string@nil"                     \
"\njumpifeq $continue_end_op GF@&type1 GF@&type2"             \
"\njumpifeq $continue_mid_op GF@&type1 string@float"          \
"\ncall $retyping_var1"                                       \
"\nlabel $continue_mid_op"                                    \
"\ncall $retyping_var2"                                       \
"\nlabel $continue_end_op"                                    \
"\npushs GF@&var1"                                            \
"\npushs GF@&var2"                                            \
"\nreturn\n"

#define FUNC_CHECK_DIV                                          \
"\nlabel $check_div"                                            \
"\npops GF@&var2"                                               \
"\npops GF@&var1"                                               \
"\ntype GF@&type1 GF@&var1"                                     \
"\ntype GF@&type2 GF@&var2"                                     \
"\njumpifeq $op_nil GF@&type1 string@nil"                       \
"\njumpifeq $op_nil GF@&type2 string@nil"                       \
"\njumpifeq $continue_mid_div GF@&type1 string@int"             \
"\njumpifeq $continue_end_div GF@&type2 string@float"           \
"\ncall $retyping_var2"                                         \
"\njump $continue_end_div"                                      \
"\nlabel $continue_mid_div"                                     \
"\ncall $retyping_var1"                                         \
"\njumpifeq $continue_end_div GF@&type2 string@float"           \
"\ncall $retyping_var2"                                         \
"\nlabel $continue_end_div"                                     \
"\njumpifeq $div_zero_error GF@&var2 int@0"                     \
"\npushs GF@&var1"                                              \
"\npushs GF@&var2"                                              \
"\nreturn"                                                      \
"\nlabel $div_zero_error"                                       \
"\nexit int@9\n"


#define FUNC_CHECK_COMP                                       \
"\nlabel $check_comp"                                         \
"\npops GF@&var2"                                             \
"\npops GF@&var1"                                             \
"\ntype GF@&type1 GF@&var1"                                   \
"\ntype GF@&type2 GF@&var2"                                   \
"\njumpifeq $continue_end_comp GF@&type1 string@nil"          \
"\njumpifeq $continue_end_comp GF@&type2 string@nil"          \
"\njumpifeq $continue_mid_comp GF@&type1 string@int"          \
"\njumpifeq $continue_end_comp GF@&type2 string@float"        \
"\ncall $retyping_var2"                                       \
"\njump $continue_end_comp"                                   \
"\nlabel $continue_mid_comp"                                  \
"\ncall $retyping_var1"                                       \
"\njumpifeq $continue_end_comp GF@&type2 string@float"        \
"\ncall $retyping_var2"                                       \
"\nlabel $continue_end_comp"                                  \
"\npushs GF@&var1"                                            \
"\npushs GF@&var2"                                            \
"\nreturn\n"

#endif // CODE_GENERATOR_H
