import dataclasses
from itertools import product

HEADER_PATH = "./../src/syntaxtree.h"

EXPRESSION_TYPE = """typedef struct Expresion {
  Expr_Type_e type;
  Syntax_u object
} Expr_s;\n"""

PRETTY_PRINTER = "void print_expr(Expr_s expr);\n"

GRAMMAR = [
        "Binary: Expr_s left, Token_s operator, Expr_s right",
        "Grouping: Expr_s expression",
        "Unary: Token_s operator, Expr_s right",
        "Literal: Token_s object"
]

VISITORS = [
        "const char*",
]

def convert_to_line(string: str) -> str:
    return string if string.endswith("\n") else string + "\n"

@dataclasses.dataclass
class Field:
    name: str
    c_type: str

    def __str__(self) -> str:
        code = f"  {self.c_type} {self.name};"
        return convert_to_line(code)

@dataclasses.dataclass
class Struct:
    name: str
    fields: list[Field]

    @property
    def type_str(self) -> str:
        return f"{self.name.capitalize()}_s"

    def __str__(self) -> str:
        code = convert_to_line(f"typedef struct {self.name} {{")
        for field in self.fields:
            code += str(field)
        code += convert_to_line(f"  bool valid;")
        code += convert_to_line(f"}} {self.name}_s;")
        return code

def parse_grammar_rule(rule: str) -> Struct:
    rule_split = rule.split(":")
    name, fields_str = rule_split[0].strip(), rule_split[1].strip()
    fields = []
    for field_str in fields_str.split(","):
        words = field_str.strip().split(" ")
        field = Field(name=words[-1], c_type=" ".join(words[:-1]))
        fields.append(field)
    return Struct(name=name, fields=fields)

def create_visitor(struct: Struct, return_t: str) -> str:
    type_fun_name = ''.join(return_t.split(' '))
    if type_fun_name.endswith("*"):
        type_fun_name = type_fun_name[:-1] + "ptr"
    return f"{return_t} visit_{struct.name.lower()}_{type_fun_name}({struct.type_str} expr);\n"

def create_expr_union(expr_types: list[Struct]) -> str:
    code = convert_to_line("typedef union Syntax {")
    for t in expr_types:
        code += convert_to_line(f"  {t.type_str} {t.name.lower()};") 
    code += convert_to_line("} Syntax_u;")
    return code

def create_expr_enum(expr_types: list[Struct]) -> str:
    code = convert_to_line("typedef enum Expr_Type {")
    for t in expr_types:
        code += convert_to_line(f"  {t.name.upper()};") 
    code += convert_to_line("} Expr_Type_e;")
    return code

# --------------------- PROGRAM START ----------------------------------------

EMPTY_LINE = convert_to_line("")
types = list(map(parse_grammar_rule, GRAMMAR))

program = [ 
           convert_to_line("#ifndef SYNTAX_TREE_H"),
           convert_to_line("#def SYNTAX_TREE_H"),
           EMPTY_LINE,
           create_expr_enum(types),
           EMPTY_LINE,
           create_expr_union(types),
           EMPTY_LINE,
           EXPRESSION_TYPE,
           EMPTY_LINE,
           ]

for t in types:
    program.extend([
        str(t),
        EMPTY_LINE,
        ])

for (t,v) in product(types, VISITORS):
    print(t,v)
    program.append(create_visitor(t,v))

program.extend([
    PRETTY_PRINTER,
    EMPTY_LINE,
    convert_to_line("#endif\n")
    ])

with open(HEADER_PATH, "w") as file:
    file.writelines(program)
