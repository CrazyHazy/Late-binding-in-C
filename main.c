#include<stdio.h>
#include<stdlib.h>
#include <string.h>

const unsigned MAX_BUF_SIZE = 100;

struct BaseStruct;

//base struct virtual table 
//yes, I don't know what is "typedef" 
struct BSVT
{
	int(*Write)(struct BaseStruct* self, const char* message);
	char*(*Read)(struct BaseStruct* self);
	void(*DeleteStruct)(struct BaseStruct* self);
};

//base stctuct (something like "interface")
struct BaseStruct
{ 
	struct BSVT vt_;	
	char* buf_;
	size_t length_;
};

int Write(struct BaseStruct* self, const char* message)
{
	return self->vt_.Write(self, message);
};

char* Read(struct BaseStruct* self)
{
	return self->vt_.Read(self);
};

void Delete(struct BaseStruct* self)
{
	self->vt_.DeleteStruct(self);
}

//sub struct 1 
struct SubStructOne
{
	struct BSVT vt_;
	char* buf_;
	size_t length_;
};

 int SubStructWrite(struct BaseStruct* self, const char* message)
{
	size_t length = strlen(message);

	if (length > MAX_BUF_SIZE)
	{
		return -1;
	}

	self->length_ = length;
	self->buf_ = realloc(self->buf_, sizeof(char) * self->length_);
	
	for (size_t i = 0; i < self->length_; i++)
	{
		self->buf_[i] = message[i];
	}

	self->buf_[self->length_] = '\0';
	return 1;
}

char* SubStructRead(struct BaseStruct* self)
{
	if (self->buf_ == NULL)
	{
		return NULL;
	}

	char* copy = (char*)malloc(sizeof(char) * self->length_);

	for (size_t i = 0; i < self->length_; i++)
	{
		copy[i] = self->buf_[i];
	}

	copy[self->length_] = '\0';
	return copy;
}

void DeleteSubStruct(struct BaseStruct* self)
{
	if (self->buf_ != NULL)
	{
		free(self->buf_);
	}
}

struct BaseStruct* CreateStructOne()
{
	struct SubStructOne* ss1 = (struct SubStructOne*)malloc(sizeof(struct SubStructOne));
	ss1->vt_.Write = SubStructWrite;
	ss1->vt_.Read = SubStructRead;
	ss1->vt_.DeleteStruct = DeleteSubStruct;
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


int SubStructWriteReverse(struct BaseStruct* self, const char* message)
{
	size_t length = strlen(message);

	if (length > MAX_BUF_SIZE)
	{
		return -1;
	}

	self->length_ = length;
	self->buf_ = realloc(self->buf_, sizeof(char) * self->length_);

	for (size_t i = 0, j = self->length_ - 1; i < self->length_; i++, j--)
	{
		self->buf_[i] = message[j];
	}

	self->buf_[self->length_] = '\0';
	return 1;
}

struct BaseStruct* CreateStructTwo()
{
	struct SubStructTwo* ss2 = (struct SubStructTwo*)malloc(sizeof(struct SubStructTwo));
	ss2->vt_.Write = SubStructWriteReverse; // it's the difference 
	ss2->vt_.Read = SubStructRead;
	ss2->vt_.DeleteStruct = DeleteSubStruct;
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

	DeleteSubStruct(s1);
	DeleteSubStruct(s2);


	return 0;
}
