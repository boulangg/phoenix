#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include <include/constant.h>
#include <asm/cpu.h>
#include <asm/io.h>
#include <boot/processor_struct.hpp>

#include <driver/input/KeyboardDevice.hpp>

struct InterruptFlags {
	bool FAST : 1;
	bool SHARED : 1;
};

extern "C" std::uint64_t readCR2();
extern "C" void generic_exception_handler(std::uint32_t irq, std::uint32_t errocode);
extern "C" void generic_interrupt_handler(std::uint32_t irq);
extern "C" void set_IDT(uint16_t limit, struct gate_desc* base);
extern "C" void enable_syscall();
extern "C" void load_syscall(uint64_t STAR,  uint64_t LSTAR, uint64_t CSTAR, uint32_t SFMASK);
extern "C" void syscall64_handler();

#define EXCEPTION_HANDLER_NAME(num) EXC_##num##_HANDLER
#define DEFINE_EXCEPTION_HANDLER(num) extern "C" void EXCEPTION_HANDLER_NAME(num)();

DEFINE_EXCEPTION_HANDLER(00);
DEFINE_EXCEPTION_HANDLER(01);
DEFINE_EXCEPTION_HANDLER(02);
DEFINE_EXCEPTION_HANDLER(03);
DEFINE_EXCEPTION_HANDLER(04);
DEFINE_EXCEPTION_HANDLER(05);
DEFINE_EXCEPTION_HANDLER(06);
DEFINE_EXCEPTION_HANDLER(07);
DEFINE_EXCEPTION_HANDLER(08);
DEFINE_EXCEPTION_HANDLER(09);
DEFINE_EXCEPTION_HANDLER(0A);
DEFINE_EXCEPTION_HANDLER(0B);
DEFINE_EXCEPTION_HANDLER(0C);
DEFINE_EXCEPTION_HANDLER(0D);
DEFINE_EXCEPTION_HANDLER(0E);
DEFINE_EXCEPTION_HANDLER(0F);
DEFINE_EXCEPTION_HANDLER(10);
DEFINE_EXCEPTION_HANDLER(11);
DEFINE_EXCEPTION_HANDLER(12);
DEFINE_EXCEPTION_HANDLER(13);
DEFINE_EXCEPTION_HANDLER(14);
DEFINE_EXCEPTION_HANDLER(15);
DEFINE_EXCEPTION_HANDLER(16);
DEFINE_EXCEPTION_HANDLER(17);
DEFINE_EXCEPTION_HANDLER(18);
DEFINE_EXCEPTION_HANDLER(19);
DEFINE_EXCEPTION_HANDLER(1A);
DEFINE_EXCEPTION_HANDLER(1B);
DEFINE_EXCEPTION_HANDLER(1C);
DEFINE_EXCEPTION_HANDLER(1D);
DEFINE_EXCEPTION_HANDLER(1E);
DEFINE_EXCEPTION_HANDLER(1F);


#define INTERRUPT_HANDLER_NAME(num) INT_##num##_HANDLER
#define DEFINE_INTERRUPT_HANDLER(num) extern "C" void INTERRUPT_HANDLER_NAME(num)();

DEFINE_INTERRUPT_HANDLER(00);
DEFINE_INTERRUPT_HANDLER(01);
DEFINE_INTERRUPT_HANDLER(02);
DEFINE_INTERRUPT_HANDLER(03);
DEFINE_INTERRUPT_HANDLER(04);
DEFINE_INTERRUPT_HANDLER(05);
DEFINE_INTERRUPT_HANDLER(06);
DEFINE_INTERRUPT_HANDLER(07);
DEFINE_INTERRUPT_HANDLER(08);
DEFINE_INTERRUPT_HANDLER(09);
DEFINE_INTERRUPT_HANDLER(0A);
DEFINE_INTERRUPT_HANDLER(0B);
DEFINE_INTERRUPT_HANDLER(0C);
DEFINE_INTERRUPT_HANDLER(0D);
DEFINE_INTERRUPT_HANDLER(0E);
DEFINE_INTERRUPT_HANDLER(0F);

class InterruptHandler {
public:
	InterruptHandler(std::string name, InterruptFlags flags, void* devID) :
		_name(name), _flags(flags), _devID(devID)
	{

	}

	virtual ~InterruptHandler() {

	}

	virtual int operator()() = 0;

	const std::string& getName() {
		return _name;
	}

	const InterruptFlags& getFlags() {
		return _flags;
	}

	void* getDevID() {
		return _devID;
	}

private:
	std::string _name;
	InterruptFlags _flags;
	void* _devID;
};

template <class Obj, int (Obj::*handler_function)()>
class InterruptHandlerClass : public InterruptHandler {
public:
	//typedef typename Obj::handler_function handler_type;
	InterruptHandlerClass(std::string name, InterruptFlags flags,
			void* devID, Obj* obj) :
				InterruptHandler(name, flags, devID), _obj(obj) {

	}

	virtual ~InterruptHandlerClass() {

	}

	virtual int operator()() override {
		return (_obj->*handler_function)();
	}

	Obj* _obj;
	//handler_type* _handler;
};

template <int (*handler_function)()>
class InterruptHandlerFunction : public InterruptHandler {
public:
	//typedef handler_function handler_type;
	InterruptHandlerFunction(std::string name, InterruptFlags flags,
			void* devID) :
				InterruptHandler(name, flags, devID) {

	}

	virtual ~InterruptHandlerFunction() {

	}

	virtual int operator()() override {
		return handler_function();
	}

	//handler_function* _handler;
};

class InterruptManager {
public:

	typedef void (*irqfunction)() ;

	static void init() {
		setupIDT();
		for (uint32_t i = 0; i < 16; i++) {
			_handlers.push_back(nullptr);
			//_sharedHandlers.push_back(std::list<InterruptHandler*>());
		}
	}

	static void setupIDT() {
		set_IDT(sizeof(idt)-1, idt);
	}

	static void setupHandlers() {
		uint8_t idt_flags = FLAG_P | FLAG_DPL3 | FLAG_INT;
		// Exception handlers
		fill_idt_descriptor_64( 0, (uint64_t)EXCEPTION_HANDLER_NAME(00), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 1, (uint64_t)EXCEPTION_HANDLER_NAME(01), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 2, (uint64_t)EXCEPTION_HANDLER_NAME(02), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 3, (uint64_t)EXCEPTION_HANDLER_NAME(03), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 4, (uint64_t)EXCEPTION_HANDLER_NAME(04), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 5, (uint64_t)EXCEPTION_HANDLER_NAME(05), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 6, (uint64_t)EXCEPTION_HANDLER_NAME(06), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 7, (uint64_t)EXCEPTION_HANDLER_NAME(07), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 8, (uint64_t)EXCEPTION_HANDLER_NAME(08), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64( 9, (uint64_t)EXCEPTION_HANDLER_NAME(09), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(10, (uint64_t)EXCEPTION_HANDLER_NAME(0A), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(11, (uint64_t)EXCEPTION_HANDLER_NAME(0B), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(12, (uint64_t)EXCEPTION_HANDLER_NAME(0C), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(13, (uint64_t)EXCEPTION_HANDLER_NAME(0D), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(14, (uint64_t)EXCEPTION_HANDLER_NAME(0E), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(15, (uint64_t)EXCEPTION_HANDLER_NAME(0F), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(16, (uint64_t)EXCEPTION_HANDLER_NAME(10), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(17, (uint64_t)EXCEPTION_HANDLER_NAME(11), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(18, (uint64_t)EXCEPTION_HANDLER_NAME(12), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(19, (uint64_t)EXCEPTION_HANDLER_NAME(13), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(20, (uint64_t)EXCEPTION_HANDLER_NAME(14), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(21, (uint64_t)EXCEPTION_HANDLER_NAME(15), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(22, (uint64_t)EXCEPTION_HANDLER_NAME(16), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(23, (uint64_t)EXCEPTION_HANDLER_NAME(17), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(24, (uint64_t)EXCEPTION_HANDLER_NAME(18), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(25, (uint64_t)EXCEPTION_HANDLER_NAME(19), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(26, (uint64_t)EXCEPTION_HANDLER_NAME(1A), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(27, (uint64_t)EXCEPTION_HANDLER_NAME(1B), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(28, (uint64_t)EXCEPTION_HANDLER_NAME(1C), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(29, (uint64_t)EXCEPTION_HANDLER_NAME(1D), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(30, (uint64_t)EXCEPTION_HANDLER_NAME(1E), SEL_KERNEL_CS, idt_flags, 1);
		fill_idt_descriptor_64(31, (uint64_t)EXCEPTION_HANDLER_NAME(1F), SEL_KERNEL_CS, idt_flags, 1);

		// PIC interrupt handlers
		fill_idt_descriptor_64(32,(uint64_t)INTERRUPT_HANDLER_NAME(00),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(33,(uint64_t)INTERRUPT_HANDLER_NAME(01),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(34,(uint64_t)INTERRUPT_HANDLER_NAME(02),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(35,(uint64_t)INTERRUPT_HANDLER_NAME(03),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(36,(uint64_t)INTERRUPT_HANDLER_NAME(04),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(37,(uint64_t)INTERRUPT_HANDLER_NAME(05),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(38,(uint64_t)INTERRUPT_HANDLER_NAME(06),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(39,(uint64_t)INTERRUPT_HANDLER_NAME(07),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(40,(uint64_t)INTERRUPT_HANDLER_NAME(08),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(41,(uint64_t)INTERRUPT_HANDLER_NAME(09),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(42,(uint64_t)INTERRUPT_HANDLER_NAME(0A),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(43,(uint64_t)INTERRUPT_HANDLER_NAME(0B),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(44,(uint64_t)INTERRUPT_HANDLER_NAME(0C),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(45,(uint64_t)INTERRUPT_HANDLER_NAME(0D),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(46,(uint64_t)INTERRUPT_HANDLER_NAME(0E),SEL_KERNEL_CS,idt_flags,1);
		fill_idt_descriptor_64(47,(uint64_t)INTERRUPT_HANDLER_NAME(0F),SEL_KERNEL_CS,idt_flags,1);

		//fill_idt_descriptor_64(32,(uint64_t)IT_32_handler,SEL_KERNEL_CS,idt_flags,1);
		//fill_idt_descriptor_64(33,(uint64_t)IT_33_handler,SEL_KERNEL_CS,idt_flags,1);
		//fill_idt_descriptor_64(14, (uint64_t)EXC_14_handler, SEL_KERNEL_CS, idt_flags, 1);

		//Clock::init();

		//IRQ_mask(0,false);
		//IRQ_mask(1,false);
	}

	static void fill_idt_descriptor_64(uint8_t index, uint64_t offset, uint16_t selector,
			uint8_t flags, uint8_t ist)
	{
		struct gate_desc* gate = &(idt[index]);

		gate->offset_1 = offset & 0xFFFF;
		gate->selector = selector;
		gate->ist = ist;
		gate->reserved_1 = 0;
		gate->flags = (flags | FLAG_P) & 0xEF;
		gate->offset_2 = (offset >> 16) & 0xFFFF;
		gate->offset_3 = (offset >> 32);
		gate->reserved_2 = 0;
	}

	static void setupSyscall() {
		enable_syscall();
		uint64_t STAR = ((((uint64_t)SEL_USER_CS_32) << 48) | (((uint64_t)SEL_KERNEL_CS) << 32) | 0);
		load_syscall(STAR, (uint64_t)syscall64_handler, 0, 0);
	}

	static void setupPIC() {
		/* Initialize the master. */
		outb(0x20, 0x11);		// Init command
		outb(0x21, 0x20);		// Set offset (IRQ0->7 use IVT[0x20->0x27])
		outb(0x21, 0x4);		// Set slave at IRQ2
		outb(0x21, 0x1);		// Set 8086 mode ?

		/* Initialize the slave. */
		outb(0xa0, 0x11);		// Init command
		outb(0xa1, 0x28);		// Set offset (IRQ8->15 use IVT[0x28->0x2F]
		outb(0xa1, 0x2);		// Set master
		outb(0xa1, 0x1);		// Set 8086 mode ?

		/* Disable all IRQs */
		outb(0x21, 0xff);
		outb(0xa1, 0xff);

		/* Ack any bogus intrs by setting the End Of Interrupt bit. */
		outb(0x20, 0x20);
		outb(0xa0, 0x20);
	}

	static void IRQ_mask(uint16_t index,bool mask){
		unsigned char m;
		unsigned char port;
		if (index < 8) {
			port = 0x21;
		} else {
			if(!mask){
				IRQ_mask(2, false);
			}
			port = 0xa1;
			index -= 8;
		}

		m = inb(port);
		if (mask) {
			m |= (1 << index);
		} else {
			m &= ~(1 << index);
		}
		outb(port, m);
	}

	static int requestIRQ(std::uint32_t irq, InterruptHandler* handler) {
		if (irq >= 16) {
			return -1;
		}
		if (!handler->getFlags().SHARED) {
			_handlers[irq] = handler;
		} else {
			_sharedHandlers[irq].push_back(handler);
		}
		IRQ_mask(irq,false);
		return 0;
	}

	static int freeIRD(std::uint32_t irq, void*devID);

	static int handleIRQ(std::uint32_t irq) {
		// Is it PIC interrupt
		if (irq < 16) {
			// Does it come from slave PIC
			if (irq >= 8) {
				outb(0xA0, 0x20);
			}
			outb(0x20, 0x20);
		}
		/*switch(irq) {
		case 0:
			Clock::tic();
			break;
		case 1:
			Keyboard::handleInterrupt();
			break;
		default:
			return 0;
		}*/
		if (_handlers[irq] != nullptr) {
			if (_handlers[irq]->getFlags().FAST) {
				(*_handlers[irq])();
			} else {
				enableInterrupt();
				(*_handlers[irq])();
				disableInterrupt();
			}
		} else {
			for (auto handler: _sharedHandlers[irq]) {
				if (handler->getFlags().FAST) {
					handler[irq]();
				} else {
					enableInterrupt();
					handler[irq]();
					disableInterrupt();
				}
			}
		}

		return 0;
	}

	static int scheduleIRQtask();

	static bool interruptEnabled();

	static void disableInterrupt() {
		cli();
	}

	static void enableInterrupt() {
		sti();
	}

	static std::vector<InterruptHandler*> _handlers;
	static std::vector< std::list<InterruptHandler*> > _sharedHandlers;

	// Exceptions handlers
	typedef void (*exception_handler_t)(std::uint32_t);

	static exception_handler_t exception_handlers[32];

	static void Exception_00_DIVZero(std::uint32_t);
	static void Exception_01_Debug(std::uint32_t);
	static void Exception_02_NMI(std::uint32_t);
	static void Exception_03_Breakpoint(std::uint32_t);
	static void Exception_04_Overflow(std::uint32_t);
	static void Exception_05_BoundRange(std::uint32_t);
	static void Exception_06_InvalidOpcode(std::uint32_t);
	static void Exception_07_DeviceNotAvailable(std::uint32_t);
	static void Exception_08_DoubleFault(std::uint32_t errorCode);
	static void Exception_10_InvalidTSS(std::uint32_t errorCode);
	static void Exception_11_SegmentNotPresent(std::uint32_t errorCode);
	static void Exception_12_StackSegmentFault(std::uint32_t errorCode);
	static void Exception_13_GPFault(std::uint32_t errorCode);
	static void Exception_14_PageFault(std::uint32_t errorCode);
	static void Exception_16_x87FP(std::uint32_t);
	static void Exception_17_AlignmentCheck(std::uint32_t errorCode);
	static void Exception_18_MachineCheck(std::uint32_t);
	static void Exception_19_SIMDFP(std::uint32_t);
	static void Exception_20_Virtualization(std::uint32_t);
	static void Exception_30_Security(std::uint32_t errorCode);
	static void Exception_Reserved(std::uint32_t); // 9, 15, 21-29, 31
};


