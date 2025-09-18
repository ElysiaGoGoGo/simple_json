python
import gdb.printing
printer = gdb.printing.RegexpCollectionPrettyPrinter("libstdc++-v6")
printer.add_printer('std::vector', '^std::vector<.*>$', 'std::vector')

class StdIteratorPrinter:
    "Print an std::iterator"
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return "Iterator pointing to: " + str(self.val.dereference())

printer.add_printer('std::iterator', '^std::_List_iterator<.*>$', StdIteratorPrinter)

gdb.printing.register_pretty_printer(gdb.current_objfile(), printer)
end
