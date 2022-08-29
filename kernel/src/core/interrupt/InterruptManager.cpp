#include "InterruptManager.hpp"

#include <core/Console.hpp>

#include <proc/ProcessScheduler.hpp>

InterruptManager::exception_handler_t InterruptManager::exception_handlers[32] = {
		InterruptManager::Exception_00_DIVZero,
		InterruptManager::Exception_01_Debug,
		InterruptManager::Exception_02_NMI,
		InterruptManager::Exception_03_Breakpoint,
		InterruptManager::Exception_04_Overflow,
		InterruptManager::Exception_05_BoundRange,
		InterruptManager::Exception_06_InvalidOpcode,
		InterruptManager::Exception_07_DeviceNotAvailable,
		InterruptManager::Exception_08_DoubleFault,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_10_InvalidTSS,
		InterruptManager::Exception_11_SegmentNotPresent,
		InterruptManager::Exception_12_StackSegmentFault,
		InterruptManager::Exception_13_GPFault,
		InterruptManager::Exception_14_PageFault,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_16_x87FP,
		InterruptManager::Exception_17_AlignmentCheck,
		InterruptManager::Exception_18_MachineCheck,
		InterruptManager::Exception_19_SIMDFP,
		InterruptManager::Exception_20_Virtualization,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_Reserved,
		InterruptManager::Exception_30_Security,
		InterruptManager::Exception_Reserved
};

std::vector<InterruptHandler*> InterruptManager::_handlers;
std::vector< std::list<InterruptHandler*> > InterruptManager::_sharedHandlers;



void generic_exception_handler(std::uint32_t irq, std::uint32_t errocode)
{
	InterruptManager::exception_handlers[irq](errocode);
}

void generic_interrupt_handler(std::uint32_t irq)
{
	InterruptManager::handleIRQ(irq);
}

void InterruptManager::Exception_00_DIVZero(std::uint32_t)
{
	cout << "INT: Divide-by-zero Error\n";
	while (1) {

	}
}

void InterruptManager::Exception_01_Debug(std::uint32_t)
{
	cout << "INT: Debug\n";
	while (1) {

	}
}

void InterruptManager::Exception_02_NMI(std::uint32_t)
{
	cout << "INT: NMI\n";
	while (1) {

	}
}
void InterruptManager::Exception_03_Breakpoint(std::uint32_t)
{
	cout << "INT: Breakpoint\n";
	while (1) {

	}
}


void InterruptManager::Exception_04_Overflow(std::uint32_t)
{
	cout << "INT: Overflow\n";
	while (1) {

	}
}


void InterruptManager::Exception_05_BoundRange(std::uint32_t)
{
	cout << "INT: Bound Range\n";
	while (1) {

	}
}


void InterruptManager::Exception_06_InvalidOpcode(std::uint32_t)
{
	cout << "INT: Invalid Opcode\n";
	while (1) {

	}
}


void InterruptManager::Exception_07_DeviceNotAvailable(std::uint32_t)
{
	cout << "INT: Device Not Available\n";
	while (1) {

	}
}


void InterruptManager::Exception_08_DoubleFault(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Double Fault\n";
	while (1) {

	}
}

void InterruptManager::Exception_10_InvalidTSS(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Invalid TSS\n";
	while (1) {

	}
}


void InterruptManager::Exception_11_SegmentNotPresent(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Segment not Present\n";
	while (1) {

	}
}


void InterruptManager::Exception_12_StackSegmentFault(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Stack Segment Fault\n";
	while (1) {

	}
}


void InterruptManager::Exception_13_GPFault(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Debug General Protection Fault\n";
	while (1) {

	}
}


void InterruptManager::Exception_14_PageFault(std::uint32_t errorCode)
{
	std::uint64_t pfa = readCR2();
	ProcessScheduler::pageFault(errorCode, (void*)pfa);
	//Console::write("INT: Page Fault\n");
}


void InterruptManager::Exception_16_x87FP(std::uint32_t)
{
	cout << "INT: x87 Floating-Point Excpetion\n";
	while (1) {

	}
}


void InterruptManager::Exception_17_AlignmentCheck(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Alignment Check\n";
	while (1) {

	}
}


void InterruptManager::Exception_18_MachineCheck(std::uint32_t)
{
	cout << "INT: Machine Check\n";
	while (1) {

	}
}


void InterruptManager::Exception_19_SIMDFP(std::uint32_t)
{
	cout << "INT: SIMDFP\n";
	while (1) {

	}
}


void InterruptManager::Exception_20_Virtualization(std::uint32_t)
{
	cout << "INT: Virtualization\n";
	while (1) {

	}
}


void InterruptManager::Exception_30_Security(std::uint32_t errorCode)
{
	(void)errorCode;
	cout << "INT: Security\n";
	while (1) {

	}
}


void InterruptManager::Exception_Reserved(std::uint32_t)
{
	cout << "INT: Rserved\n";
	while (1) {

	}
}
