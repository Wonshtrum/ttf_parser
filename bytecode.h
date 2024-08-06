void read_instructions(Slice instructions) TOGGLE_H_IMPL({
	int args = 0;
	int len = instructions.len;
	u8* ptr = instructions.ptr;
	for (int i=0; i<len; i++) {
		u8 byte = ptr[i];
		if (args) {
			printf("%02x", byte);
			if (--args) {
				printf(", ");
			} else {
				printf("\n");
			}
			continue;
		}
		const char* n;
		int base = 0;
		int bits = 0;
		switch (byte) {
			case 0x00 ... 0x01: n = "SVTCA"; base=0x00; bits=1; break;
			case 0x02 ... 0x03: n = "SPVTCA"; base=0x02; bits=1; break;
			case 0x04 ... 0x05: n = "SFVTCA"; base=0x04; bits=1; break;
			case 0x06 ... 0x07: n = "SPVTL"; base=0x06; bits=1; break;
			case 0x08 ... 0x09: n = "SFVTL"; base=0x08; bits=1; break;
			case 0x0A: n = "SPVFS"; break;
			case 0x0C: n = "GPV"; break;
			case 0x0D: n = "GFV"; break;
			case 0x0F: n = "ISECT"; break;
			case 0x10: n = "SRP0"; break;
			case 0x11: n = "SRP1"; break;
			case 0x12: n = "SRP2"; break;
			case 0x13: n = "SZP0"; break;
			case 0x14: n = "SZP1"; break;
			case 0x15: n = "SZP2"; break;
			case 0x16: n = "SZPS"; break;
			case 0x17: n = "SLOOP"; break;
			case 0x18: n = "RTG"; break;
			case 0x19: n = "RTHG"; break;
			case 0x1A: n = "SMD"; break;
			case 0x1B: n = "ELSE"; break;
			case 0x1C: n = "JMPR"; break;
			case 0x1D: n = "SCVTCI"; break;
			case 0x1E: n = "SSWCI"; break;
			case 0x1F: n = "SSW"; break;
			case 0x20: n = "DUP"; break;
			case 0x21: n = "POP"; break;
			case 0x22: n = "CLEAR"; break;
			case 0x23: n = "SWAP"; break;
			case 0x24: n = "DEPTH"; break;
			case 0x25: n = "CINDEX"; break;
			case 0x26: n = "MINDEX"; break;
			case 0x27: n = "ALIGNPTS"; break;
			case 0x29: n = "UTP"; break;
			case 0x2A: n = "LOOPCALL"; break;
			case 0x2B: n = "CALL"; break;
			case 0x2C: n = "FDEF"; break;
			case 0x2D: n = "ENDF"; break;
			case 0x2E ... 0x2F: n = "MDAP"; base=0x2E; bits=1; break;
			case 0x30 ... 0x31: n = "IUP"; base=0x30; bits=1; break;
			case 0x32 ... 0x33: n = "SHP"; base=0x32; bits=1; break;
			case 0x34 ... 0x35: n = "SHC"; base=0x34; bits=1; break;
			case 0x36 ... 0x37: n = "SHZ"; base=0x36; bits=1; break;
			case 0x38: n = "SHPIX"; break;
			case 0x39: n = "IP"; break;
			case 0x3A ... 0x3B: n = "MSIRP"; base=0x3A; bits=1; break;
			case 0x3C: n = "ALIGNRP"; break;
			case 0x3D: n = "RTDG"; break;
			case 0x3E ... 0x3F: n = "MIAP"; base=0x3E; bits=1; break;
			case 0x40: n = "NPUSHB"; args=ptr[++i]; base=args; bits=8; break;
			case 0x41: n = "NPUSHW"; args=2*(ptr[++i]); base=args/2; bits=8; break;
			case 0x43: n = "RS"; break;
			case 0x44: n = "WCVTP"; break;
			case 0x45: n = "RCVT"; break;
			case 0x46 ... 0x47: n = "GC"; base=0x46; bits=1; break;
			case 0x48: n = "SCFS"; break;
			case 0x49 ... 0x4A: n = "MD"; base=0x49; bits=1; break;
			case 0x4B: n = "MPPEM"; break;
			case 0x4C: n = "MPS"; break;
			case 0x4D: n = "FLIPON"; break;
			case 0x4E: n = "FLIPOFF"; break;
			case 0x4F: n = "DEBUG"; break;
			case 0x50: n = "LT"; break;
			case 0x51: n = "LTEQ"; break;
			case 0x52: n = "GT"; break;
			case 0x53: n = "GTEQ"; break;
			case 0x54: n = "EQ"; break;
			case 0x55: n = "NEQ"; break;
			case 0x56: n = "ODD"; break;
			case 0x57: n = "EVEN"; break;
			case 0x58: n = "IF"; break;
			case 0x59: n = "EIF"; break;
			case 0x5A: n = "AND"; break;
			case 0x5B: n = "OR"; break;
			case 0x5C: n = "NOT"; break;
			case 0x5D: n = "DELTAP1"; break;
			case 0x5E: n = "SDB"; break;
			case 0x5F: n = "SDS"; break;
			case 0x60: n = "ADD"; break;
			case 0x61: n = "SUB"; break;
			case 0x62: n = "DIV"; break;
			case 0x63: n = "MUL"; break;
			case 0x64: n = "ABS"; break;
			case 0x65: n = "NEG"; break;
			case 0x66: n = "FLOOR"; break;
			case 0x67: n = "CEILING"; break;
			case 0x68 ... 0x6B: n = "ROUND"; base=0x68; bits=2; break;
			case 0x6C ... 0x6F: n = "NROUND"; base=0x6C; bits=2; break;
			case 0x70: n = "WCVTF"; break;
			case 0x71: n = "DELTAP2"; break;
			case 0x72: n = "DELTAP3"; break;
			case 0x73: n = "DELTAC1"; break;
			case 0x74: n = "DELTAC2"; break;
			case 0x75: n = "DELTAC3"; break;
			case 0x76: n = "SROUND"; break;
			case 0x77: n = "S45ROUND"; break;
			case 0x78: n = "JROT"; break;
			case 0x79: n = "JROF"; break;
			case 0x7A: n = "ROFF"; break;
			case 0x7C: n = "RUTG"; break;
			case 0x7D: n = "RDTG"; break;
			case 0x7E: n = "SANGW"; break;
			case 0x80: n = "FLIPPT"; break;
			case 0x81: n = "FLIPRGON"; break;
			case 0x82: n = "FLIPRGOFF"; break;
			case 0x85: n = "SCANCTRL"; break;
			case 0x86 ... 0x87: n = "SDPVTL"; base=0x86; bits=1; break;
			case 0x88: n = "GETINFO"; break;
			case 0x89: n = "IDEF"; break;
			case 0x8A: n = "ROLL"; break;
			case 0x8B: n = "MAX"; break;
			case 0x8C: n = "MIN"; break;
			case 0x8D: n = "SCANTYPE"; break;
			case 0x8E: n = "INSTCTRL"; break;
			case 0xB0 ... 0xB7: n = "PUSHB"; base=0xB0; bits=3; args=byte-0xB0+1; break;
			case 0xB8 ... 0xBF: n = "PUSHW"; base=0xB8; bits=3; args=2*(byte-0xB8+1); break;
			case 0xC0 ... 0xDF: n = "MDRP"; base=0xC0; bits=5; break;
			case 0xE0 ... 0xFF: n = "MIRP"; base=0xE0; bits=5; break;
		}
		if (bits == 8) {
			MRC_INFO("%s[%02x]", n, base);
		} else if (bits) {
			MRC_INFO("%s[%0*b]", n, bits, byte-base);
		} else {
			MRC_INFO("%s[]", n);
		}
	}
})
