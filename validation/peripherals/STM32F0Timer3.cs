//
// Copyright (c) 2010-2020 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Time;

namespace Antmicro.Renode.Peripherals.Timers
{
    // This class does not implement advanced-control timers interrupts
    [AllowedTranslations(AllowedTranslation.ByteToDoubleWord)]
    public class STM32F0Timer3 : LimitTimer, IDoubleWordPeripheral
    {
        public STM32F0Timer3(Machine machine, long frequency, uint initialLimit) : base(machine.ClockSource, frequency, limit: initialLimit,  direction: Direction.Ascending, enabled: false, autoUpdate: false)
        {
            IRQ = new GPIO();
            this.initialLimit = initialLimit;

            LimitReached += delegate
            {
                if(updateDisable.Value)
                {
                    return;
                }
                if(updateInterruptEnable.Value)
                {
                    IRQ.Set();
                    updateInterruptFlag = true;
                }
                Limit = autoReloadValue;
            };

            var registersMap = new Dictionary<long, DoubleWordRegister>
            {
                {(long)Registers.Control1, new DoubleWordRegister(this)
                    .WithFlag(0, writeCallback: (_, val) => Enabled = val, valueProviderCallback: _ => Enabled, name: "Counter enable (CEN)")
                    .WithFlag(1, out updateDisable, name: "Update disable (UDIS)")
                    .WithFlag(2, out updateRequestSource, name: "Update request source (URS)")
                    .WithFlag(3, writeCallback: (_, val) => Mode = val ? WorkMode.OneShot : WorkMode.Periodic, valueProviderCallback: _ => Mode == WorkMode.OneShot, name: "One-pulse mode (OPM)")
                    .WithFlag(4, writeCallback: (_, val) => Direction = val ? Direction.Descending : Direction.Ascending, valueProviderCallback: _ => Direction == Direction.Descending, name: "Direction (DIR)")
                    .WithTag("Center-aligned mode selection (CMS)", 5, 2)
                    .WithFlag(7, out autoReloadPreloadEnable, name: "Auto-reload preload enable (APRE)")
                    .WithTag("Clock Division (CKD)", 8, 2)
                    .WithReservedBits(10, 22)
                },

                {(long)Registers.Control2, new DoubleWordRegister(this)
                    .WithReservedBits(0, 2)
                    .WithTag("Capture/compare DMA selection (CCDS)", 3, 1)
                    .WithTag("Master mode selection (MMS)", 4, 3)
                    .WithTag("TI1 selection (TI1S)", 7, 1)
                    .WithReservedBits(8, 24)
                },

                {(long)Registers.SlaveModeControl, new DoubleWordRegister(this)
                    .WithTag("Slave mode selection (SMS)", 0, 3)
                    .WithTag("OCREF clear selection (OCCS)", 3, 1)
                    .WithTag("Trigger selection (TS)", 4, 3)
                    .WithTag("Master/Slave mode (MSM)", 7, 1)
                    .WithTag("External trigger filter (ETF)", 8, 3)
                    .WithTag("External trigger prescaler (ETPS)", 12, 2)
                    .WithTag("External clock enable (ECE)", 14, 1)
                    .WithTag("External trigger polarity (ETP)", 15, 1)                    
                    .WithReservedBits(16, 16)
                },

                {(long)Registers.DmaOrInterruptEnable, new DoubleWordRegister(this)
                    .WithFlag(0, out updateInterruptEnable, name: "Update interrupt enable (UIE)")
                    .WithTag("Capture/Compare 1 interrupt enable (CC1IE)", 1, 1)
                    .WithTag("Capture/Compare 2 interrupt enable (CC2IE)", 2, 1)
                    .WithTag("Capture/Compare 3 interrupt enable (CC3IE)", 3, 1)
                    .WithTag("Capture/Compare 4 interrupt enable (CC4IE)", 4, 1)
                    .WithReservedBits(5, 1)
                    .WithTag("Trigger interrupt enable (TIE)", 6, 1)
                    .WithReservedBits(7, 1)
                    .WithTag("Update DMA request enable (UDE)", 8, 1)
                    .WithTag("Capture/Compare 1 DMA request enable (CC1DE)", 9, 1)
                    .WithTag("Capture/Compare 2 DMA request enable (CC2DE)", 10, 1)
                    .WithTag("Capture/Compare 3 DMA request enable (CC3DE)", 11, 1)
                    .WithTag("Capture/Compare 4 DMA request enable (CC4DE)", 12, 1)
                    .WithReservedBits(13, 1)
                    .WithTag("Trigger DMA request enable (TDE)", 14, 1)
                    .WithReservedBits(15, 17)
                },

                {(long)Registers.Status, new DoubleWordRegister(this)
                    .WithFlag(0, FieldMode.Read | FieldMode.WriteZeroToClear,
                        writeCallback: (_, val) =>
                        {
                            if(!val)
                            {
                                updateInterruptFlag = false;
                                IRQ.Unset();
                            }
                        },
                        valueProviderCallback: (_) =>
                        {
                            return updateInterruptFlag;
                        },
                        name: "Update interrupt flag (UIF)")
                    // These write callbacks are here only to prevent from very frequent logging.
                    .WithValueField(1, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 1 interrupt flag (CC1IF)")
                    .WithValueField(2, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 2 interrupt flag (CC2IF)")
                    .WithValueField(3, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 3 interrupt flag (CC3IF)")
                    .WithValueField(4, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 4 interrupt flag (CC4IF)")
                    .WithReservedBits(5, 1)
                    .WithValueField(6, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Trigger interrupt flag (TIE)")
                    .WithReservedBits(7, 2)
                    .WithValueField(9, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 1 overcapture flag (CC1OF)")
                    .WithValueField(10, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 2 overcapture flag (CC2OF)")
                    .WithValueField(11, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 3 overcapture flag (CC3OF)")
                    .WithValueField(12, 1, FieldMode.WriteZeroToClear, writeCallback: (_, __) => {}, name: "Capture/Compare 4 overcapture flag (CC4OF)")
                    .WithReservedBits(13, 18)
                },

                {(long)Registers.EventGeneration, new DoubleWordRegister(this)
                    .WithFlag(0, FieldMode.WriteOneToClear, writeCallback: (_, val) =>
                    {
                        if(updateDisable.Value)
                        {
                            return;
                        }
                        if(Direction == Direction.Ascending)
                        {
                            Value = 0;
                        }
                        else if(Direction == Direction.Descending)
                        {
                            Value = autoReloadValue;
                        }
                        if(!updateRequestSource.Value && updateInterruptEnable.Value)
                        {
                            IRQ.Set();
                            updateInterruptFlag = true;
                        }
                    }, name: "Update generation (UG)")
                    .WithTag("Capture/compare 1 generation (CC1G)", 1, 1)
                    .WithTag("Capture/compare 2 generation (CC2G)", 2, 1)
                    .WithTag("Capture/compare 3 generation (CC3G)", 3, 1)
                    .WithTag("Capture/compare 4 generation (CC4G)", 4, 1)
                    .WithReservedBits(5, 1)
                    .WithTag("Trigger generation (TG)", 6, 1)
                    .WithReservedBits(7, 25)
                },

                {(long)Registers.CaptureOrCompareMode1, new DoubleWordRegister(this)
                    .WithTag("Capture/Compare 1 selection (CC1S)", 0, 2)
                    .WithTag("Output compare 1 fast enable (OC1FE)", 2, 1)
                    .WithTag("Output compare 1 preload enable (OC1PE)", 3, 1)
                    .WithTag("Output compare 1 mode (OC1M)", 4, 3)
                    .WithTag("Output compare 1 clear enable (OC1CE)", 7, 1)
                    .WithTag("Capture/Compare 2 selection (CC1S)", 8, 2)
                    .WithTag("Output compare 2 fast enable (OC1FE)", 10, 1)
                    .WithTag("Output compare 2 preload enable (OC1PE)", 11, 1)
                    .WithTag("Output compare 2 mode (OC1M)", 12, 3)
                    .WithTag("Output compare 2 clear enable (OC1CE)", 15, 1)
                    .WithReservedBits(16, 6)
                },

                {(long)Registers.CaptureOrCompareMode2, new DoubleWordRegister(this)
                    .WithTag("Capture/Compare 3 selection (CC1S)", 0, 2)
                    .WithTag("Output compare 3 fast enable (OC1FE)", 2, 1)
                    .WithTag("Output compare 3 preload enable (OC1PE)", 3, 1)
                    .WithTag("Output compare 3 mode (OC1M)", 4, 3)
                    .WithTag("Output compare 3 clear enable (OC1CE)", 7, 1)
                    .WithTag("Capture/Compare 4 selection (CC1S)", 8, 2)
                    .WithTag("Output compare 4 fast enable (OC1FE)", 10, 1)
                    .WithTag("Output compare 4 preload enable (OC1PE)", 11, 1)
                    .WithTag("Output compare 4 mode (OC1M)", 12, 3)
                    .WithTag("Output compare 4 clear enable (OC1CE)", 15, 1)
                    .WithReservedBits(16, 16)
                },

                {(long)Registers.CaptureOrCompareEnable, new DoubleWordRegister(this)
                    .WithTag("Capture/Compare 1 output enable (CC1E)", 0, 1)
                    .WithTag("Capture/Compare 1 output polarity (CC1P)", 1, 1)
                    .WithReservedBits(2, 1)
                    .WithTag("Capture/Compare 1 output polarity (CC1NP)", 3, 1)
                    .WithTag("Capture/Compare 2 output enable (CC2E)", 4, 1)
                    .WithTag("Capture/Compare 2 output polarity (CC2P)", 5, 1)
                    .WithReservedBits(6, 1)
                    .WithTag("Capture/Compare 2 output polarity (CC2NP)", 7, 1)
                    .WithTag("Capture/Compare 3 output enable (CC3E)", 8, 1)
                    .WithTag("Capture/Compare 3 output polarity (CC3P)", 9, 1)
                    .WithReservedBits(10, 1)
                    .WithTag("Capture/Compare 3 output polarity (CC3NP)", 11, 1)
                    .WithTag("Capture/Compare 4 output enable (CC4E)", 12, 1)
                    .WithTag("Capture/Compare 4 output polarity (CC4P)", 13, 1)
                    .WithReservedBits(14, 1)
                    .WithTag("Capture/Compare 4 output polarity (CC4NP)", 15, 1)
                },

                {(long)Registers.Counter, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => Value = val, valueProviderCallback: _ => (uint)Value, name: "Counter value (CNT)")
                },

                {(long)Registers.Prescaler, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => Divider = (int)val + 1, valueProviderCallback: _ => (uint)Divider - 1, name: "Prescaler value (PSC)")
                },

                {(long)Registers.AutoReload, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) =>
                    {
                        autoReloadValue = val;
                        if(!autoReloadPreloadEnable.Value)
                        {
                            Limit = autoReloadValue;
                        }
                    }, valueProviderCallback: _ => autoReloadValue, name: "Counter value (CNT)")
                },

                {(long)Registers.CaptureOrCompare1, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => channel1 = val, valueProviderCallback: _ => channel1, name: "Capture/compare register 1 (CCR1)")
                },

                {(long)Registers.CaptureOrCompare2, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => channel2 = val, valueProviderCallback: _ => channel2, name: "Capture/compare register 2 (CCR2)")
                },

                {(long)Registers.CaptureOrCompare3, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => channel3 = val, valueProviderCallback: _ => channel3, name: "Capture/compare register 3 (CCR3)")
                },

                {(long)Registers.CaptureOrCompare4, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => channel4 = val, valueProviderCallback: _ => channel4, name: "Capture/compare register 4 (CCR4)")
                }
            };

            registers = new DoubleWordRegisterCollection(this, registersMap);
            Reset();

            EventEnabled = true;
        }

        public uint ReadDoubleWord(long offset)
        {
            return registers.Read(offset);
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            registers.Write(offset, value);
        }

        public override void Reset()
        {
            base.Reset();
            registers.Reset();
            IRQ.Set(false);
            autoReloadValue = initialLimit;
            Limit = initialLimit;
            updateInterruptFlag = false;
        }

        public GPIO IRQ { get; private set; }

        public long Size = 0x400;

        private uint initialLimit;
        private uint channel1;
        private uint channel2;
        private uint channel3;
        private uint channel4;
        private uint autoReloadValue;
        private bool updateInterruptFlag;
        private readonly IFlagRegisterField updateDisable;
        private readonly IFlagRegisterField updateRequestSource;
        private readonly IFlagRegisterField updateInterruptEnable;
        private readonly IFlagRegisterField autoReloadPreloadEnable;
        private readonly DoubleWordRegisterCollection registers;

        private enum Registers : long
        {
            Control1 = 0x0,
            Control2 = 0x04,
            SlaveModeControl = 0x08,
            DmaOrInterruptEnable = 0x0C,
            Status = 0x10,
            EventGeneration = 0x14,
            CaptureOrCompareMode1 = 0x18,
            CaptureOrCompareMode2 = 0x1C,
            CaptureOrCompareEnable = 0x20,
            Counter = 0x24,
            Prescaler = 0x28,
            AutoReload = 0x2C,
            // gap intended
            CaptureOrCompare1 = 0x34,
            CaptureOrCompare2 = 0x38,
            CaptureOrCompare3 = 0x3C,
            CaptureOrCompare4 = 0x40,
            // gap intended
            DmaControl = 0x48,
            DmaAddressForFullTransfer = 0x4C
        }
    }
}

    