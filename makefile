TARGET:=main

SRCS:=$(wildcard *.cpp)

OBJS:=$(patsubst %.cpp,%.o,$(SRCS))

LIBS:= -L/lib64/mysql/ -lpthread -ljsoncpp -lmysqlclient

$(TARGET):$(OBJS)
	$(CXX) $^ -o $@ $(LIBS)


clean:
	$(RM) $(SRCS) $(OBJS)

.PHONY:clean