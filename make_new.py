import click

def cap(day):
    if day.startswith('twenty'):
        return 'Twenty' + day[6:].capitalize()

def header(day):
    return f'''#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class {cap(day)}{{

    std::ifstream fin;

    void load();

public:
    {cap(day)}(const std::string&);

    int part_one();
    int part_two();
}};

'''

def implementation(day):
    return f'''#include "{day}.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

{cap(day)}::{cap(day)}(const std::string& filename) : fin(filename) {{
    this->load();
    hdr::print("Day {cap(day)}");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    t0 = Tempus::time();
    x = this->part_two();
    t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}}

void {cap(day)}::load(){{}}

int {cap(day)}::part_one(){{
    return -1;
}}

int {cap(day)}::part_two(){{
    return -1;
}}
'''

@click.command()
@click.argument('day')
def cli(day):

    with open(f'include/{day}.hpp', 'w', encoding='utf-8', newline='\n') as fout:
        _ = fout.write(header(day))

    with open(f'src/{day}.cpp', 'w', encoding='utf-8', newline='\n') as fout:
        _ = fout.write(implementation(day))

    with open(f'data/{day}.txt', 'w', encoding='utf-8', newline='\n') as fout:
        pass

    with open(f'data/example/{day}.txt', 'w', encoding='utf-8', newline='\n') as fout:
        pass

    with open('include/main.hpp', 'a', encoding='utf-8', newline='\n') as fout:
        _ = fout.write(f'#include "{day}.hpp"\n')

if __name__ == '__main__':
    cli()