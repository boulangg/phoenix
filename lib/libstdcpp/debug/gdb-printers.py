import gdb

class SummaryAndFieldIterator:
    """
    Iterator to first go through a list of summariser functions,
    then display all the fields in the object in order
    """
    def __init__ (self, obj, summarize):
        self.obj = obj;
        self.summarize = summarize
        self.count = 0

    def __iter__(self):
        return self

    def __next__(self):

        if (self.count >= len(self.summarize)):
            raise StopIteration
        else:
            name = self.summarize[self.count]
            retVal = self.obj[name]
            result = "[%s]" % name, retVal

        self.count += 1
        return result

    next = __next__

class StdStringPrinter(object):
    "Print a std::string"

    def __init__(self, val):
        self.val = val

    #def children(self):
    #    return SummaryAndFieldIterator(self.val, [self.get_string])

    #def get_string(self):
    #    return "_size", "_capacity", "_data"
        
    def to_string(self):
        return self.val["_data"]

    def display_hint(self):
        return 'string'


import gdb.printing

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter(
        "my_library")
    pp.add_printer('string', '^std::string', StdStringPrinter)
    return pp


import gdb.printing

gdb.printing.register_pretty_printer(
    gdb.current_objfile(),
    build_pretty_printer())

