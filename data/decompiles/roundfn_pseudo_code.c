
void FUN_1405ccaa0(uint *param_1,int param_2,undefined1 *param_3,undefined1 *param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar8 = CONCAT31(CONCAT21(CONCAT11(*param_3,param_3[1]),param_3[2]),param_3[3]) ^ *param_1;
  uVar4 = CONCAT31(CONCAT21(CONCAT11(param_3[4],param_3[5]),param_3[6]),param_3[7]) ^ param_1[1];
  uVar2 = CONCAT31(CONCAT21(CONCAT11(param_3[8],param_3[9]),param_3[10]),param_3[0xb]) ^ param_1[2];
  uVar1 = CONCAT31(CONCAT21(CONCAT11(param_3[0xc],param_3[0xd]),param_3[0xe]),param_3[0xf]) ^
          param_1[3];
  uVar3 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar1 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[4];
  uVar6 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar1 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar2 & 0xff) * 4) ^ param_1[5];
  uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar8 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar4 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[6];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar4 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar2 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[7];
  uVar2 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar6 & 0xff) * 4) ^ param_1[8];
  uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[9];
  uVar4 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar3 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar6 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[10];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar5 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0xb];
  uVar3 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar4 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0xc];
  uVar6 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[0xd];
  uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0xe];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar4 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar2 & 0xff) * 4) ^ param_1[0xf];
  uVar2 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar6 & 0xff) * 4) ^ param_1[0x10];
  uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[0x11];
  uVar4 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar3 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar6 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x12];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar5 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0x13];
  uVar3 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar4 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0x14];
  uVar6 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[0x15];
  uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x16];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar4 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar2 & 0xff) * 4) ^ param_1[0x17];
  uVar2 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar6 & 0xff) * 4) ^ param_1[0x18];
  uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[0x19];
  uVar4 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar3 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar6 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x1a];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar5 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0x1b];
  uVar3 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar4 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0x1c];
  uVar7 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[0x1d];
  uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x1e];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar4 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar2 & 0xff) * 4) ^ param_1[0x1f];
  uVar2 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar7 & 0xff) * 4) ^ param_1[0x20];
  uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar7 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[0x21];
  uVar6 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar3 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar7 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x22];
  uVar5 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar7 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar5 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0x23];
  uVar4 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar5 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0x24];
  uVar3 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar6 & 0xff) * 4) ^ param_1[0x25];
  uVar1 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
          *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar5 & 0xff) * 4) ^ param_1[0x26];
  uVar2 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar6 >> 0x10) & 0xff) * 4) ^
          *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
          *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar2 & 0xff) * 4) ^ param_1[0x27];
  if (10 < param_2) {
    uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
            *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0x28];
    uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar4 >> 0x10) * 4) ^
            *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x29];
    uVar6 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar4 >> 8) * 4) ^
            *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar2 & 0xff) * 4) ^ param_1[0x2a];
    uVar2 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar3 >> 8) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[0x2b];
    uVar4 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar2 >> 0x10) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0x2c];
    uVar3 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar5 >> 0x10) * 4) ^
            *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar6 & 0xff) * 4) ^ param_1[0x2d];
    uVar1 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
            *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar2 & 0xff) * 4) ^ param_1[0x2e];
    uVar2 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar6 >> 0x10) & 0xff) * 4) ^
            *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
            *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[0x2f];
    if (0xc < param_2) {
      uVar5 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar1 >> 8) * 4) ^
              *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar2 >> 0x10) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar4 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar3 & 0xff) * 4) ^ param_1[0x30];
      uVar8 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar4 >> 0x10) * 4) ^
              *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar3 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar1 & 0xff) * 4) ^ param_1[0x31];
      uVar6 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar4 >> 8) * 4) ^
              *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar1 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar2 & 0xff) * 4) ^ param_1[0x32];
      uVar2 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar3 >> 8) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar4 & 0xff) * 4) ^ param_1[0x33];
      uVar4 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar6 >> 8) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar2 >> 0x10) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar5 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar8 & 0xff) * 4) ^ param_1[0x34];
      uVar3 = *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar5 >> 0x10) * 4) ^
              *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar2 >> 8) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar8 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar6 & 0xff) * 4) ^ param_1[0x35];
      uVar1 = *(uint *)(&DAT_1416b2ed0 + (ulonglong)(byte)(uVar5 >> 8) * 4) ^
              *(uint *)(&DAT_1416b2a90 + (ulonglong)(byte)(uVar8 >> 0x10) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar6 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + ((ulonglong)uVar2 & 0xff) * 4) ^ param_1[0x36];
      uVar2 = *(uint *)(&DAT_1416b2ed0 + ((ulonglong)(uVar8 >> 8) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2a90 + ((ulonglong)(uVar6 >> 0x10) & 0xff) * 4) ^
              *(uint *)(&DAT_1416b2670 + (ulonglong)(uVar2 >> 0x18) * 4) ^
              *(uint *)(&DAT_1416b3300 + (ulonglong)(uVar5 & 0xff) * 4) ^ param_1[0x37];
    }
  }
  param_1 = param_1 + param_2 * 4;
  uVar5 = *(uint *)(&DAT_1416b3740 + ((ulonglong)(uVar2 >> 0x10) & 0xff) * 4) & 0xff0000 ^
          *(uint *)(&DAT_1416b3740 + ((ulonglong)(uVar1 >> 8) & 0xff) * 4) & 0xff00 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(uVar4 >> 0x18) * 4) & 0xff000000 ^
          (uint)(byte)(&DAT_1416b3740)[(ulonglong)(uVar3 & 0xff) * 4] ^ *param_1;
  param_4[3] = (char)uVar5;
  *param_4 = (char)(uVar5 >> 0x18);
  param_4[1] = (char)(uVar5 >> 0x10);
  param_4[2] = (char)(uVar5 >> 8);
  uVar5 = *(uint *)(&DAT_1416b3740 + (ulonglong)(byte)(uVar4 >> 0x10) * 4) & 0xff0000 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(byte)(uVar2 >> 8) * 4) & 0xff00 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(uVar3 >> 0x18) * 4) & 0xff000000 ^
          (uint)(byte)(&DAT_1416b3740)[((ulonglong)uVar1 & 0xff) * 4] ^ param_1[1];
  param_4[7] = (char)uVar5;
  param_4[4] = (char)(uVar5 >> 0x18);
  param_4[5] = (char)(uVar5 >> 0x10);
  param_4[6] = (char)(uVar5 >> 8);
  uVar5 = *(uint *)(&DAT_1416b3740 + (ulonglong)(byte)(uVar3 >> 0x10) * 4) & 0xff0000 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(byte)(uVar4 >> 8) * 4) & 0xff00 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(uVar1 >> 0x18) * 4) & 0xff000000 ^
          (uint)(byte)(&DAT_1416b3740)[((ulonglong)uVar2 & 0xff) * 4] ^ param_1[2];
  param_4[0xb] = (char)uVar5;
  param_4[8] = (char)(uVar5 >> 0x18);
  param_4[9] = (char)(uVar5 >> 0x10);
  param_4[10] = (char)(uVar5 >> 8);
  uVar1 = *(uint *)(&DAT_1416b3740 + ((ulonglong)(uVar1 >> 0x10) & 0xff) * 4) & 0xff0000 ^
          *(uint *)(&DAT_1416b3740 + ((ulonglong)(uVar3 >> 8) & 0xff) * 4) & 0xff00 ^
          *(uint *)(&DAT_1416b3740 + (ulonglong)(uVar2 >> 0x18) * 4) & 0xff000000 ^
          (uint)(byte)(&DAT_1416b3740)[(ulonglong)(uVar4 & 0xff) * 4] ^ param_1[3];
  param_4[0xf] = (char)uVar1;
  param_4[0xc] = (char)(uVar1 >> 0x18);
  param_4[0xd] = (char)(uVar1 >> 0x10);
  param_4[0xe] = (char)(uVar1 >> 8);
  return;
}

