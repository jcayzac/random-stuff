import os
import thread
import threading


class ThreadedDict:
    """
    A dictonary-like object where the data is thread-dependent.
    """
    def _checkcreate(self):
        try:
            self.local.data
        except AttributeError:
            self.local.data = {}

    def __init__(self, dict=None, **kwargs):
        self.local = threading.local()
        self._checkcreate()
        if dict is not None:
            self.update(dict)
        if len(kwargs):
            self.update(kwargs)

    def __repr__(self):
        self._checkcreate()
        return repr(self.local.data)

    def __cmp__(self, dict):
        self._checkcreate()
        if isinstance(dict, ThreadedDict):
            return cmp(self.local.data, dict.local.data)
        else:
            return cmp(self.local.data, dict)

    def __len__(self):
        self._checkcreate()
        return len(self.local.data)

    def __getitem__(self, key):
        self._checkcreate()
        if key in self.local.data:
            return self.local.data[key]
        if hasattr(self.__class__, "__missing__"):
            return self.__class__.__missing__(self, key)
        raise KeyError(key)

    def __setitem__(self, key, item):
        self._checkcreate()
        self.local.data[key] = item

    def __delitem__(self, key):
        self._checkcreate()
        del self.local.data[key]

    def clear(self):
        self._checkcreate()
        self.local.data.clear()

    def copy(self):
        self._checkcreate()
        if self.__class__ is ThreadedDict:
            return ThreadedDict(self.local.data.copy())
        import copy
        data = self.local.data
        try:
            self.local.data = {}
            c = copy.copy(self)
        finally:
            self.local.data = data
        c.update(self)
        return c

    def keys(self):
        self._checkcreate()
        return self.local.data.keys()

    def items(self):
        self._checkcreate()
        return self.local.data.items()

    def iteritems(self):
        self._checkcreate()
        return self.local.data.iteritems()

    def iterkeys(self):
        self._checkcreate()
        return self.local.data.iterkeys()

    def itervalues(self):
        self._checkcreate()
        return self.local.data.itervalues()

    def values(self):
        self._checkcreate()
        return self.local.data.values()

    def has_key(self, key):
        self._checkcreate()
        return self.local.data.has_key(key)

    def update(self, dict=None, **kwargs):
        self._checkcreate()
        if dict is None:
            pass
        elif isinstance(dict, ThreadedDict):
            self.update(dict.local.data, kwargs)
            return
        elif isinstance(dict, type({})) or not hasattr(dict, 'items'):
            self.local.data.update(dict)
        else:
            for k, v in dict.items():
                self[k] = v
        if len(kwargs):
            self.local.data.update(kwargs)

    def get(self, key, failobj=None):
        self._checkcreate()
        if not self.has_key(key):
            return failobj
        return self[key]

    def setdefault(self, key, failobj=None):
        self._checkcreate()
        if not self.has_key(key):
            self[key] = failobj
        return self[key]

    def pop(self, key, *args):
        self._checkcreate()
        return self.local.data.pop(key, *args)

    def popitem(self):
        self._checkcreate()
        return self.local.data.popitem()

    def __contains__(self, key):
        self._checkcreate()
        return key in self.local.data

    @classmethod
    def fromkeys(cls, iterable, value=None):
        d = cls()
        for key in iterable:
            d[key] = value
        return d

    def __iter__(self):
        self._checkcreate()
        return iter(self.local.data)


class ThreadedEnviron(ThreadedDict):
    """
    A replacement for os._Environ that allows thread-dependent environment.
    Note that it doesn't do os._Environ's putenv()/unsetenv() magic.
    """
    def __init__(self, environ):
        ThreadedDict.__init__(self, environ)
    def copy(self):
        self._checkcreate()
        return dict(self)

def start_new_thread(func, environ=None, *args, **kwargs):
    """
    Start a new thread with a new environment.
    If environ is None, the new thread will inherit its parent's
    environment.
    """
    if environ is None:
        environ = os.environ.copy()

    def x(func, environ, *args, **kwargs):
        os.environ.clear()
        os.environ.update(environ)
        return func(*args, **kwargs)

    return thread.start_new_thread(x, tuple([func, environ]+list(args)), kwargs)

# Install our new class
os.environ = ThreadedEnviron(os.environ)

# Sample usage:
import os
import sys
import time
def a():
    os.environ['A'] = '1'
    s = sorted(os.environ.items())
    for k, v in s:
        time.sleep(0)
        print "A: %s=%s" % (k ,v)

def b():
    os.environ['B'] = '1'
    del os.environ['PATH']
    s = sorted(os.environ.items())
    for k, v in s:
        time.sleep(0)
        print "B: %s=%s" % (k ,v)

def c():
    os.environ['C'] = '1'
    start_new_thread(b)
    start_new_thread(a)

print "Launched threads: %s" % str([
    start_new_thread(f)
    for f in [a, b, c]
])
