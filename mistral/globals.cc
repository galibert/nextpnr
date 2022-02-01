/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2021  gatecat <gatecat@ds0.me>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "log.h"
#include "nextpnr.h"
#include "util.h"

NEXTPNR_NAMESPACE_BEGIN

void Arch::create_dsp(int x, int y)
{
    BelId bel = add_bel(x, y, id_cyclonev_dsp, id_cyclonev_dsp);
    add_bel_pin(bel, id_accumulate, PORT_IN,
                get_port(CycloneV::DSP, x, y, -1, CycloneV::ACCUMULATE, -1));
    for(int i=0; i < 4; i++)
        add_bel_pin(bel, id(stringf("aclr[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, -1, CycloneV::ACLR, i));
    for(int i=0; i < 6; i++)
        add_bel_pin(bel, id(stringf("clkin[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, -1, CycloneV::CLKIN, i));
    for(int i=0; i < 9; i++) {
        add_bel_pin(bel, id(stringf("dataina[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 0, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainb[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 1, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainc[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 2, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("dataind[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 3, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("dataine[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 4, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainf[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 5, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("dataing[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 6, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainh[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 7, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("dataini[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 8, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainj[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 9, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("dataink[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 10, CycloneV::DATAIN, i));
        add_bel_pin(bel, id(stringf("datainl[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, 11, CycloneV::DATAIN, i));
    }
    for(int i=0; i < 3; i++)
        add_bel_pin(bel, id(stringf("enable[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, -1, CycloneV::ENABLE, i));
    add_bel_pin(bel, id_loadconst, PORT_IN,
                get_port(CycloneV::DSP, x, y, -1, CycloneV::LOADCONST, -1));
    add_bel_pin(bel, id_negate, PORT_IN,
                get_port(CycloneV::DSP, x, y, -1, CycloneV::NEGATE, -1));
    for(int i=0; i < 74; i++)
        add_bel_pin(bel, id(stringf("result[%d]", i)), PORT_OUT,
                    get_port(CycloneV::DSP, x, y, -1, CycloneV::RESULT, i));
    add_bel_pin(bel, id_sub, PORT_IN,
                get_port(CycloneV::DSP, x, y, -1, CycloneV::SUB, -1));
    for(int i=0; i < 8; i++) {
        static const int unk[8] = { 30,31, 62, 63, 94, 95, 126, 127 };
        add_bel_pin(bel, id(stringf("unk_in[%d]", i)), PORT_IN,
                    get_port(CycloneV::DSP, x, y, -1, CycloneV::UNK_IN, unk[i]));
    }
}

void Arch::create_clkbuf(int x, int y)
{
    for (int z = 0; z < 4; z++) {
        if (z != 2)
            continue; // TODO: why do other Zs not work?
        // For now we only consider the input path from general routing, other inputs like dedicated clock pins are
        // still a TODO
        BelId bel = add_bel(x, y, id(stringf("CLKBUF[%d]", z)), id_MISTRAL_CLKENA);
        add_bel_pin(bel, id_A, PORT_IN, get_port(CycloneV::CMUXHG, x, y, -1, CycloneV::CLKIN, z));
        add_bel_pin(bel, id_Q, PORT_OUT, get_port(CycloneV::CMUXHG, x, y, z, CycloneV::CLKOUT));
        // TODO: enable pin
        bel_data(bel).block_index = z;
    }
}

bool Arch::is_clkbuf_cell(IdString cell_type) const
{
    return cell_type == id_MISTRAL_CLKENA || cell_type == id_MISTRAL_CLKBUF;
}

void Arch::create_hps_mpu_general_purpose(int x, int y)
{
    BelId gp_bel =
            add_bel(x, y, id_cyclonev_hps_interface_mpu_general_purpose, id_cyclonev_hps_interface_mpu_general_purpose);
    for (int i = 0; i < 32; i++) {
        add_bel_pin(gp_bel, id(stringf("gp_in[%d]", i)), PORT_IN,
                    get_port(CycloneV::HPS_MPU_GENERAL_PURPOSE, x, y, -1, CycloneV::GP_IN, i));
        add_bel_pin(gp_bel, id(stringf("gp_out[%d]", i)), PORT_OUT,
                    get_port(CycloneV::HPS_MPU_GENERAL_PURPOSE, x, y, -1, CycloneV::GP_OUT, i));
    }
}

void Arch::create_control(int x, int y)
{
    BelId oscillator_bel = add_bel(x, y, id_cyclonev_oscillator, id_cyclonev_oscillator);
    add_bel_pin(oscillator_bel, id("oscena"), PORT_IN, get_port(CycloneV::CTRL, x, y, -1, CycloneV::OSC_ENA, -1));
    add_bel_pin(oscillator_bel, id("clkout"), PORT_OUT, get_port(CycloneV::CTRL, x, y, -1, CycloneV::CLK_OUT, -1));
    add_bel_pin(oscillator_bel, id("clkout1"), PORT_OUT, get_port(CycloneV::CTRL, x, y, -1, CycloneV::CLK_OUT1, -1));
}

NEXTPNR_NAMESPACE_END
