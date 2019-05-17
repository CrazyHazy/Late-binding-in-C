#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct BaseStruct;

//base struct virtual table 
//yes, I don't know what is "typedef" 
struct BSVT
{
	void(*Write)(struct BaseStruct* self, const char* message);
	char*(*Read)(struct BaseStruct* self);
	void(*Delete)(struct BaseStruct* self);
};

//base stctuct (something like "interface")
struct BaseStruct
{
	struct BSVT vt_;
	char* buf_;
	size_t length_;
};

void Write(struct BaseStruct* self, const char* message)
{
	self->vt_.Write(self, message);
};

char* Read(struct BaseStruct* self)
{
	return self->vt_.Read(self);
};

void Delete(struct BaseStruct* self)
{
	self->vt_.Delete(self);
}

//sub struct 1 
struct SubStructOne
{
	struct BSVT vt_;
	char* buf_;
	size_t length_;
};

void SubStructWrite(struct BaseStruct* self, const char* message)
{
	self->length_ = strlen(message);
	self->buf_ = (char*)realloc(self->buf_, sizeof(char) * (self->length_ + 1));
	strcpy(self->buf_, message);
	self->buf_[self->length_] = '\0';
}

char* SubStructRead(struct BaseStruct* self)
{
	char* copy = (char*)malloc(sizeof(char) * (self->length_ + 1));
	strcpy(copy, self->buf_);
	copy[self->length_] = '\0';

	return copy;
}

void DeleteSubStruct(struct BaseStruct* self)
{
	free(self->buf_);
}

struct BaseStruct* CreateStructOne()
{
	struct SubStructOne* ss1 = (struct SubStructOne*)malloc(sizeof(struct SubStructOne));
	ss1->vt_.Write = SubStructWrite;
	ss1->vt_.Read = SubStructRead;
	ss1->vt_.Delete = DeleteSubStruct;
	ss1->buf_ = NULL;
	ss1->length_ = 0;

	return (struct BaseStruct*)ss1;
};

// sub struct 2 
struct SubStructTwo
{
	struct BSVT vt_;
	char* buf_;
	size_t length_;
};

void SubStructWriteReverse(struct BaseStruct* self, const char* message)
{
	self->length_ = strlen(message);
	self->buf_ = (char*)realloc(self->buf_, sizeof(char) * (self->length_ + 1));
	strcpy(self->buf_, message);
	self->buf_[self->length_] = '\0';
}

struct BaseStruct* CreateStructTwo()
{
	struct SubStructTwo* ss2 = (struct SubStructTwo*)malloc(sizeof(struct SubStructTwo));
	ss2->vt_.Write = SubStructWriteReverse; // it's the difference 
	ss2->vt_.Read = SubStructRead;
	ss2->vt_.Delete = DeleteSubStruct;
	ss2->buf_ = NULL;
	ss2->length_ = 0;

	return (struct BaseStruct*)ss2;
};



int main(int argc, char **argv)
{

	struct BaseStruct* s1 = CreateStructOne();
	struct BaseStruct* s2 = CreateStructTwo();

	Write(s1, "hello");
	Write(s2, "hello");

	char* b_s1 = Read(s1);
	char* b_s2 = Read(s2);

	Write(s1, "olleH");
	Write(s2, "olleH");

	Delete(s1);
	Delete(s2);


	return 0;
}
