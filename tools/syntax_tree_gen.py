import dataclasses

HEADER_PATH = "./../src/syntaxtree.h"
EXPRESSION_TYPE = """typedef struct Expresion {
  Expr_Type_e type;
  Syntax_u object
} Expr_s"""
GRAMMAR = [
        "Binary: Expr_s left, Token_s operator, Expr_s right",
        "Grouping: Expr_s expression",
        "Unary: Token_s operator, Expr_s right",
        "Literal: Token_s object"
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

    def __str__(self) -> str:
        code = convert_to_line(f"typedef struct {self.name} {{")
        for field in self.fields:
            code += str(field)
        code += convert_to_line(f"bool valid;")
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


def create_struct(name: str, fields: str) -> str:
    struct = convert_to_line(f"typedef {name} {{")
    fields_list = fields.split(",")
    return struct

with open(HEADER_PATH, "w") as file:
    file.writelines([
        convert_to_line("#ifndef SYNTAX_TREE_H"),
        convert_to_line("#def SYNTAX_TREE_H")
        ])

    print(create_struct("Binary"

    file.writelines([
        convert_to_line("#endif\n")
        ])
