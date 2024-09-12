import gdb

class SummaryAndFieldIterator:
    """
    Iterator to first go through a list of summariser functions,
    then display all the fields in the object in order
    """
    def __init__ (self, obj, summaries):
        self.count = 0
        self.obj = obj;
        self.summaries = summaries;
        self.keys = sorted(obj.type.iterkeys())

    def __iter__(self):
        return self

    def __next__(self):

        #if (self.count >= len(self.keys) + len(self.summaries)):
        if (self.count >=len(self.summaries)):
            raise StopIteration
        elif self.count < len(self.summaries):

            name, retVal = self.summaries[self.count]()
            # FIXME: this doesn't seem to work when a string is returned
            # in retVal?
            result = "[%s]" % name, retVal

        else:
            field = self.count - len(self.summaries)
            result = self.keys[field], self.obj[self.keys[field]]

        self.count += 1
        return result

    next = __next__


class FieldIterator:
    """
    Iterator that display all the fields in the object in order
    """
    def __init__ (self, obj):
        self.count = 0
        self.obj = obj;
        #self.keys = sorted(obj.type.iterkeys())

    def __iter__(self):
        return self

    def __next__(self):
        if self.count < 3:
            self.count += 1
            return "test" + str(self.count), "test"
        else:
            raise StopIteration

    next = __next__

class StdStringPrinter(object):
    "Print a std::string"

    def __init__(self, val):
        self.val = val
        
    #def children(self):
        #value = (("test", "test"), ("test2", "test"))
        #return iter(value)
        #return iter(FieldIterator())
        #self.val))

    def children(self):
        return SummaryAndFieldIterator(self.val, [self.get_string_field, self.get_size_field, self.get_capacity_field])

    def get_string_field(self):
        return "string", self.val["_data"]
    def get_size_field(self):
        return "size", self.val["_size"]
    def get_capacity_field(self):
        return "capacity", self.val["_capacity"]
        
    def to_string(self):
        return self.val["_data"]

    def display_hint(self):
        return 'string'

# Register printers
def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("libstdcpp")
    pp.add_printer('custom::string', '^std::string$', StdStringPrinter)
    return pp

# Autoload
gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printer())
