# Make release
################################################################################

exe=ribbongen.exe
paths=G:/Programmation/SDL2-2.0.0/i686-w64-mingw32 G:/Programmation/glew-1.9.0
deps=mingw32 SDL2main SDL2 opengl32
compile_args=-DGLEW_STATIC -DNDEBUG
link_args=-static-libgcc -static-libstdc++
src_root=src
subdirs=glew

#===============================================================================
CXX=g++
inc=$(foreach p,$(paths),-I$(p)/include)
libs=$(foreach p,$(paths),-L$(p)/lib)
CXXFLAGS=-Wall -W -pedantic -O2 -std=c++0x -std=gnu++0x $(inc) $(compile_args)
LXXFLAGS=$(libs) $(foreach d,$(deps),-l$(d)) $(link_args)
OBJDIR=bin
# VPATH from dir list
dirs=$(src_root) $(foreach x,$(subdirs),$(src_root)/$(x))
null=#nothing
space=$(null) $(null)
VPATH=$(subst $(space),:,$(dirs))
# All cpp and c
files=$(foreach p,$(dirs),$(wildcard $(p)/*.cpp $(p)/*.c))
objs=$(addprefix $(OBJDIR)/,$(addsuffix .o,$(notdir $(basename $(files)))))

all: $(OBJDIR)/$(exe)
	@echo Done.

$(OBJDIR)/$(exe): $(objs)
	@echo Linking...
	@$(CXX) -o $@ $^ $(LXXFLAGS)

$(OBJDIR)/%.o: %.cpp
	@echo Compiling: $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: %.c
	@echo Compiling: $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $<