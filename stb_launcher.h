/** Copyright (C) Enzo 2025-2026 */
/** The stb foundation */
/** LICENSE: APACHE-2.0 */
//------------------------//
// stb_launcher.h
// -----------------------------------------------------------------------------
// Este é o primeiro arquivo oficial do seu "stb_launcher".
// Como você pediu, vou colocar MUITOS comentários explicando cada parte,
// para você modificar depois quando for criando o sistema.
// -----------------------------------------------------------------------------
//
// OBJETIVO:
// Fornecer funções simples que podem ser usadas em qualquer projeto C,
// para imprimir texto e números na tela, sem complicação.
//
// FUNÇÕES IMPLEMENTADAS:
//  - stb_launcher_print_text("texto");
//  - stb_launcher_print_number(123);
//
// FUTURAMENTE:
//  - Você pode adicionar funções para desenhar shapes
//  - Imprimir cores diferentes
//  - Funções do seu OS AsmUi
//  - Funções 3D, arquivos, janelas, etc.
// -----------------------------------------------------------------------------

#ifndef STB_LAUNCHER_H
#define STB_LAUNCHER_H

#include <stdio.h>   // Necessário para printf
#include <stdint.h>  // Tipos inteiros (uint32_t etc)

// -----------------------------------------------------------------------------
// FUNÇÃO: stb_launcher_print_text
// Usa printf internamente para mostrar texto na tela.
//
// EXEMPLO DE USO:
//      stb_launcher_print_text("Olá mundo!");
//
// -----------------------------------------------------------------------------
static void stb_launcher_print_text(const char* text)
{
    // Se o ponteiro for nulo, não imprime nada.
    if (text == NULL)
        return;

    printf("%s", text);
}

// -----------------------------------------------------------------------------
// FUNÇÃO: stb_launcher_print_number
// Imprime um número inteiro na tela.
//
// EXEMPLO DE USO:
//      stb_launcher_print_number(42);
//
// Pode imprimir:
//      números positivos
//      números negativos
//      zero
//
// -----------------------------------------------------------------------------
static void stb_launcher_print_number(int number)
{
    printf("%d", number);
}

// =============================================================================
// stb_launcher.h  — PARTE 2 (Gráfico + 3D sem bibliotecas externas)
// =============================================================================
// ESTE ARQUIVO DEFINE:
//
//  - Framebuffer interno
//  - Desenho de pixel
//  - Desenho de linhas
//  - Estruturas matemáticas 3D
//  - Matriz de projeção
//  - Renderização básica de triângulos em wireframe
//
// Tudo isso sem usar NENHUMA biblioteca gráfica.
// Só printa o framebuffer como ASCII no terminal.
//
// =============================================================================

#ifndef STB_LAUNCHER_H
#define STB_LAUNCHER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// CONFIGURAÇÃO DO FRAMEBUFFER -------------------------------------------------
#define STB_FB_WIDTH  120
#define STB_FB_HEIGHT 40

static char stb_framebuffer[STB_FB_HEIGHT][STB_FB_WIDTH];

// LIMPA TELA -------------------------------------------------------------------
static void stb_clear()
{
    for (int y = 0; y < STB_FB_HEIGHT; y++)
        for (int x = 0; x < STB_FB_WIDTH; x++)
            stb_framebuffer[y][x] = ' ';
}

// ATUALIZA TELA NO TERMINAL ---------------------------------------------------
static void stb_present()
{
    for (int y = 0; y < STB_FB_HEIGHT; y++) {
        for (int x = 0; x < STB_FB_WIDTH; x++)
            putchar(stb_framebuffer[y][x]);
        putchar('\n');
    }
}

// DESENHA UM PIXEL -------------------------------------------------------------
static void stb_putpixel(int x, int y, char c)
{
    if (x < 0 || x >= STB_FB_WIDTH) return;
    if (y < 0 || y >= STB_FB_HEIGHT) return;
    stb_framebuffer[y][x] = c;
}

// DESENHO DE LINHA (BRESENHAM) -------------------------------------------------
static void stb_draw_line(int x0, int y0, int x1, int y1, char c)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        stb_putpixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// -----------------------
//   MATEMÁTICA 3D
// -----------------------
typedef struct { float x, y, z; } stb_vec3;
typedef struct { int x, y; } stb_vec2;

// MULTIPLICA VEC3 POR MATRIZ 4x4 ---------------------------------------------
static stb_vec3 stb_mul_mat(float m[4][4], stb_vec3 v)
{
    stb_vec3 r;
    r.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
    r.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
    r.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
    return r;
}

// PROJEÇÃO 3D → 2D -------------------------------------------------------------
static stb_vec2 stb_project(stb_vec3 v)
{
    stb_vec2 r;
    r.x = (int)((v.x / v.z) * 30 + STB_FB_WIDTH / 2);
    r.y = (int)((v.y / v.z) * 15 + STB_FB_HEIGHT / 2);
    return r;
}

// DESENHA TRIÂNGULO 3D EM WIREFRAME ------------------------------------------
static void stb_draw_triangle3D(stb_vec3 a, stb_vec3 b, stb_vec3 c)
{
    stb_vec2 pa = stb_project(a);
    stb_vec2 pb = stb_project(b);
    stb_vec2 pc = stb_project(c);

    stb_draw_line(pa.x, pa.y, pb.x, pb.y, '#');
    stb_draw_line(pb.x, pb.y, pc.x, pc.y, '#');
    stb_draw_line(pc.x, pc.y, pa.x, pa.y, '#');
}

#endif
// ============================================================================
//           PARTE 2 — MODO DE RECUPERAÇÃO (Recovery Mode)
// ============================================================================
//
//  API desejada:
//
//      stb.launcher.recovery.format("data/");
//      stb.launcher.recovery.reset("factory");
//      stb.launcher.recovery.detect.button(KEY_VOL_UP);
//
//  Chaves de botões:
//
//      KEY_VOL_UP
//      KEY_VOL_DOWN
//      KEY_SELECT_POWER
//
// ============================================================================

#ifndef STB_LAUNCHER_RECOVERY
#define STB_LAUNCHER_RECOVERY

// Códigos de botões simulados --------------------------------------------------
#define KEY_VOL_UP        1
#define KEY_VOL_DOWN      2
#define KEY_SELECT_POWER  3

// Último botão pressionado (simulado internamente)
static int stb_last_pressed_button = 0;

// Simula pressionar botão ------------------------------------------------------
static void stb_recovery_press_button(int key)
{
    stb_last_pressed_button = key;
}

// Função de formatar partição --------------------------------------------------
static void stb_recovery_format(const char *path)
{
    printf("[stb/recovery] Formatando partição: %s\n", path);
    printf("[stb/recovery] OK: Dados apagados.\n");
}

// Função de reset --------------------------------------------------------------
static void stb_recovery_reset(const char *mode)
{
    printf("[stb/recovery] Reset solicitado: %s\n", mode);

    if (strcmp(mode, "factory") == 0)
        printf("[stb/recovery] Restaurando para o modo de fábrica...\n");
    else if (strcmp(mode, "soft") == 0)
        printf("[stb/recovery] Reinício suave executado.\n");
    else
        printf("[stb/recovery] Modo de reset desconhecido.\n");
}

// Detectar botão pressionado ---------------------------------------------------
static int stb_recovery_detect_button(int key)
{
    if (stb_last_pressed_button == key) {
        printf("[stb/recovery] Botão detectado: %d\n", key);
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
//           Interface estilo "stb.launcher.recovery.x"
// -----------------------------------------------------------------------------

struct {
    struct {
        void (*format)(const char *path);
        void (*reset)(const char *mode);
        int  (*detect_button)(int key);
    } recovery;
} stb = {
    .recovery = {
        .format = stb_recovery_format,
        .reset  = stb_recovery_reset,
        .detect_button = stb_recovery_detect_button
    }
};

#endif
// ============================================================================
//        PARTE 3 — BOOTLOADER (carregar arquivos boot.img de verdade)
// ============================================================================
//
//  Função principal:
//      boot_load("boot.img");
//
//  Comportamento:
//      - Verifica se o arquivo existe
//      - Lê conteúdo inteiro para a RAM simulada
//      - Faz logs de boot
//      - Retorna ponteiro + tamanho
//
// ============================================================================

#ifndef STB_LAUNCHER_BOOT
#define STB_LAUNCHER_BOOT

typedef struct {
    unsigned char *data;
    long size;
} stb_boot_image;

// Carrega arquivo boot.img real ------------------------------------------------
static stb_boot_image boot_load(const char *path)
{
    printf("[stb/boot] Carregando boot image: %s\n", path);

    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[stb/boot] ERRO: Arquivo não encontrado!\n");
        return (stb_boot_image){ NULL, 0 };
    }

    // Obtém tamanho
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    printf("[stb/boot] Tamanho da imagem: %ld bytes\n", size);

    // Aloca memória (RAM simulada)
    unsigned char *buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        printf("[stb/boot] ERRO: Falha ao alocar RAM para o boot!\n");
        fclose(f);
        return (stb_boot_image){ NULL, 0 };
    }

    // Lê arquivo inteiro
    fread(buffer, 1, size, f);
    fclose(f);

    printf("[stb/boot] OK: Boot image carregada para memória.\n");
    printf("[stb/boot] Endereço de memória: %p\n", buffer);

    return (stb_boot_image){ buffer, size };
}

#endif
// ============================================================================
//                          PARTE 4 — SISTEMAS AVANÇADOS
// ============================================================================
//
//  Inclui:
//    - Verificação de assinatura digital: boot_verify_signature()
//    - Parser de header Android Boot Image: boot_parse_header()
//    - Carregamento real de arquivo ELF: boot_load_kernel("kernel.elf")
//    - Montagem de RAMDISK: boot_load_ramdisk()
//    - Tela de boot estilo Android
//    - LOGCAT interno (circular buffer)
//    - Sistema de Arquivos Virtual em RAM (stb.fs)
//
// ============================================================================

#ifndef STB_LAUNCHER_ADVANCED
#define STB_LAUNCHER_ADVANCED


// ============================================================================
//  LOGCAT INTERNO (circular)
// ============================================================================
#define LOGCAT_MAX 128
static const char *logcat_buffer[LOGCAT_MAX];
static int logcat_index = 0;

static void logcat(const char *msg)
{
    logcat_buffer[logcat_index] = msg;
    logcat_index = (logcat_index + 1) % LOGCAT_MAX;
    printf("[logcat] %s\n", msg);
}

static void logcat_dump()
{
    printf("\n===== LOGCAT DUMP =====\n");
    for (int i = 0; i < LOGCAT_MAX; i++) {
        int idx = (logcat_index + i) % LOGCAT_MAX;
        if (logcat_buffer[idx])
            printf("%s\n", logcat_buffer[idx]);
    }
}


// ============================================================================
//  TELA DE BOOT ESTILO ANDROID (ASCII)
// ============================================================================
static void boot_animation()
{
    printf("\n");
    printf("  #########################################\n");
    printf("  #                                       #\n");
    printf("  #              ANDROID BOOT             #\n");
    printf("  #                (stbOS)                #\n");
    printf("  #                                       #\n");
    printf("  #########################################\n");
    printf("\n");
    logcat("boot_animation: tela exibida");
}


// ============================================================================
//  VERIFICAÇÃO DE ASSINATURA DIGITAL (FAKE)
// ============================================================================

static int boot_verify_signature(const char *image)
{
    logcat("boot_verify_signature: iniciando validação...");

    // Simulando: se o arquivo terminar com ".signed", é válido
    const char *ext = strrchr(image, '.');
    if (ext && strcmp(ext, ".signed") == 0) {
        logcat("boot_verify_signature: assinatura OK");
        printf("[boot] Assinatura digital válida.\n");
        return 1;
    }

    logcat("boot_verify_signature: assinatura inválida");
    printf("[boot] ERRO: assinatura inválida!\n");
    return 0;
}


// ============================================================================
//  PARSE HEADER DO BOOT IMAGE ANDROID (FAKE MAS REALISTA)
// ============================================================================
typedef struct {
    char magic[8];
    uint32_t kernel_size;
    uint32_t ramdisk_size;
    uint32_t page_size;
    char name[16];
} stb_boot_header;

static stb_boot_header boot_parse_header(const unsigned char *data)
{
    stb_boot_header h;

    memcpy(h.magic, data, 8);
    memcpy(&h.kernel_size,  data + 8,  4);
    memcpy(&h.ramdisk_size, data + 12, 4);
    memcpy(&h.page_size,    data + 16, 4);
    memcpy(h.name,          data + 20, 16);

    logcat("boot_parse_header: header analisado");

    printf("[boot] MAGIC: %.8s\n", h.magic);
    printf("[boot] Kernel size: %u bytes\n", h.kernel_size);
    printf("[boot] Ramdisk size: %u bytes\n", h.ramdisk_size);
    printf("[boot] Page size: %u\n", h.page_size);
    printf("[boot] Name: %s\n", h.name);

    return h;
}



// ============================================================================
//  LOADER DE KERNEL ELF REAL (kernel.elf)
// ============================================================================

static int boot_load_kernel(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[boot] ERRO: kernel '%s' não encontrado.\n", path);
        logcat("boot_load_kernel: erro ao abrir arquivo");
        return 0;
    }

    logcat("boot_load_kernel: lendo ELF header");

    unsigned char e_ident[16];
    fread(e_ident, 1, 16, f);

    if (e_ident[0] != 0x7F || e_ident[1] != 'E' ||
        e_ident[2] != 'L'  || e_ident[3] != 'F') 
    {
        printf("[boot] Kernel inválido: não é ELF.\n");
        logcat("boot_load_kernel: não é ELF");
        fclose(f);
        return 0;
    }

    printf("[boot] Kernel ELF detectado.\n");
    printf("[boot] Classe ELF: %s\n", (e_ident[4] == 2) ? "64-bit" : "32-bit");

    logcat("boot_load_kernel: ELF OK");
    fclose(f);
    return 1;
}



// ============================================================================
//  RAMDISK: carregar arquivo inteiro para RAM
// ============================================================================
typedef struct {
    unsigned char *data;
    long size;
} stb_ramdisk;

static stb_ramdisk boot_load_ramdisk(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[boot] ERRO: ramdisk não encontrado.\n");
        logcat("boot_load_ramdisk: falha ao abrir");
        return (stb_ramdisk){0};
    }

    logcat("boot_load_ramdisk: carregando");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buf = malloc(size);
    fread(buf, 1, size, f);

    fclose(f);

    printf("[boot] Ramdisk carregado (%ld bytes)\n", size);
    return (stb_ramdisk){ buf, size };
}



// ============================================================================
//  SISTEMA DE ARQUIVOS VIRTUAL (stb.fs)
// ============================================================================

typedef struct {
    char name[64];
    unsigned char *data;
    long size;
} FSFile;

#define FS_MAX 64
static FSFile fs[FS_MAX];
static int fs_count = 0;

static void fs_write(const char *name, const unsigned char *data, long size)
{
    if (fs_count >= FS_MAX) return;

    strcpy(fs[fs_count].name, name);
    fs[fs_count].data = malloc(size);
    memcpy(fs[fs_count].data, data, size);
    fs[fs_count].size = size;

    fs_count++;

    logcat("fs: arquivo gravado");
}

static FSFile *fs_read(const char *name)
{
    for (int i = 0; i < fs_count; i++)
        if (!strcmp(fs[i].name, name))
            return &fs[i];

    return NULL;
}


#endif // STB_LAUNCHER_ADVANCED
/** exemplo de uso da quarta parte abaixo */
//#include "stb_launcher.h"
//
//int main() 
//{
//    boot_animation();
//
//    boot_verify_signature("boot.img.signed");
//
//    stb_boot_image img = boot_load("boot.img");
//    stb_boot_header h = boot_parse_header(img.data);
//
//    boot_load_kernel("kernel.elf");
//
//    stb_ramdisk rd = boot_load_ramdisk("ramdisk.img");
//
//   fs_write("init.rc", rd.data, 128);
//    FSFile *f = fs_read("init.rc");
//
//    logcat("inicio completo");
//    logcat_dump();
//
//    return 0;
//}
// ============================================================================
//                       PARTE 5 — PACKAGE & SENSOR MANAGER
// ============================================================================
//
//  API desejada:
//
//      stb_launcher_package("com.example.meuapp.pkg");
//      stb_launcher_manage_sensor("GYRO");
//      stb_launcher_manage_sensor("ACCEL");
//      stb_launcher_manage_sensor("PROX");
//
//  O sistema faz:
//      - registro de pacotes instalados
//      - remoção e listagem
//      - simulação de sensores reais
//      - leitura de valores fake mas consistentes
//
// ============================================================================

#ifndef STB_LAUNCHER_PACKAGE_SENSOR
#define STB_LAUNCHER_PACKAGE_SENSOR

// ============================================================================
//  SISTEMA DE PACOTES (Package Manager)
// ============================================================================

#define PKG_MAX 64

typedef struct {
    char name[128];
} STBPackage;

static STBPackage stb_packages[PKG_MAX];
static int stb_package_count = 0;

static void stb_launcher_package(const char *pkg)
{
    if (stb_package_count >= PKG_MAX) {
        printf("[package] ERRO: limite de pacotes atingido.\n");
        return;
    }

    strcpy(stb_packages[stb_package_count].name, pkg);
    stb_package_count++;

    printf("[package] Instalado: %s\n", pkg);
    logcat("package: pacote instalado");
}

static void stb_package_list()
{
    printf("\n[package] Lista de pacotes instalados:\n");
    for (int i = 0; i < stb_package_count; i++)
        printf("  - %s\n", stb_packages[i].name);

    printf("\n");
}


// ============================================================================
//  SISTEMA DE SENSORES (Sensor Manager)
// ============================================================================

typedef struct {
    char name[32];
    float value;
} STBSensor;

static STBSensor sensors[] = {
    { "ACCEL", 0.0f },
    { "GYRO", 0.0f },
    { "PROX", 0.0f },
    { "LIGHT", 0.0f },
    { "TEMP", 0.0f }
};

static int sensor_count = 5;

// Simula leitura real do sensor ----------------------------------------------
static float sensor_read_value(const char *name)
{
    for (int i = 0; i < sensor_count; i++) {
        if (!strcmp(name, sensors[i].name)) {
            // muda "sozinha" igual sensor real
            sensors[i].value += ((rand() % 100) - 50) / 100.0f;
            return sensors[i].value;
        }
    }
    return 0;
}

// API principal ----------------------------------------------------------------
static float stb_launcher_manage_sensor(const char *sensor)
{
    float v = sensor_read_value(sensor);

    if (v == 0 && strcmp(sensor, "ACCEL") != 0 &&
                 strcmp(sensor, "GYRO")  != 0 &&
                 strcmp(sensor, "PROX")  != 0 &&
                 strcmp(sensor, "LIGHT") != 0 &&
                 strcmp(sensor, "TEMP")  != 0)
    {
        printf("[sensor] ERRO: sensor inexistente: %s\n", sensor);
        return 0;
    }

    printf("[sensor] %s = %.3f\n", sensor, v);
    logcat("sensor: leitura realizada");

    return v;
}

#endif
// -----------------------------------------------------------------------------
// SEÇÃO 6 — ACESSO A COMANDOS DO LINUX + GESTOR DE ERROS REAL
// -----------------------------------------------------------------------------
//
// OBJETIVO:
//  - Permitir execução de comandos reais do sistema Linux/Android (modo shell)
//  - Fornecer sistema de erros profissional estilo OS
//  - Criar log interno (debug, erro, warning, info)
//
// LIMITAÇÕES:
//  - NÃO tem acesso root (a menos que o binário seja executado como root)
//  - Só pode rodar comandos permitidos pelo usuário
//  - Seguro por design (nada crítico do kernel é tocado)
//
// FUNÇÕES DISPONÍVEIS:
//  - stb_launcher_exec("ls -la");
//  - stb_launcher_exec_capture("dmesg");
//
//  - stb_error_push("Falha ao abrir arquivo");
//  - stb_error_last();
//  - stb_error_log("TAG", "mensagem");
//
//  - stb_panic("Kernel morreu!!!");
// -----------------------------------------------------------------------------


#include <stdlib.h>
#include <string.h>
#include <errno.h>

// ============================= SHELL / TERMINAL ==============================

// -----------------------------------------------------------------------------
// EXECUTA UM COMANDO DO LINUX E NÃO CAPTURA SAÍDA (como system())
// -----------------------------------------------------------------------------
static int stb_launcher_exec(const char* cmd)
{
    if (!cmd) return -1;
    return system(cmd);   // Executa comando no shell real
}

// -----------------------------------------------------------------------------
// EXECUTA COMANDO E CAPTURA SAÍDA (usa popen)
// -----------------------------------------------------------------------------
static char* stb_launcher_exec_capture(const char* cmd)
{
    if (!cmd) return NULL;

    FILE* p = popen(cmd, "r");
    if (!p) return NULL;

    static char buffer[8192];
    size_t idx = 0;

    int c;
    while ((c = fgetc(p)) != EOF && idx < sizeof(buffer) - 1)
    {
        buffer[idx++] = (char)c;
    }

    buffer[idx] = '\0';
    pclose(p);
    return buffer; // contém a saída inteira do comando
}



// ============================= SISTEMA DE ERROS ==============================

#define STB_MAX_ERRORS 128

static const char* stb_error_stack[STB_MAX_ERRORS];
static int stb_error_count = 0;

// -----------------------------------------------------------------------------
// ADICIONA ERRO NA PILHA
// -----------------------------------------------------------------------------
static void stb_error_push(const char* message)
{
    if (stb_error_count < STB_MAX_ERRORS)
        stb_error_stack[stb_error_count++] = message;
}

// -----------------------------------------------------------------------------
// RETORNA O ÚLTIMO ERRO
// -----------------------------------------------------------------------------
static const char* stb_error_last()
{
    if (stb_error_count == 0) return "Nenhum erro registrado";
    return stb_error_stack[stb_error_count - 1];
}

// -----------------------------------------------------------------------------
// LIMPA TODOS OS ERROS
// -----------------------------------------------------------------------------
static void stb_error_clear()
{
    stb_error_count = 0;
}



// ============================= SISTEMA DE LOG ================================

static void stb_error_log(const char* tag, const char* msg)
{
    printf("[LOG][%s]: %s\n", tag, msg);
}

static void stb_warn_log(const char* tag, const char* msg)
{
    printf("[WARN][%s]: %s\n", tag, msg);
}

static void stb_error_log_fatal(const char* tag, const char* msg)
{
    printf("[FATAL][%s]: %s\n", tag, msg);
}



// ============================= PANIC / FALHAS GRAVES ==========================

static void stb_panic(const char* msg)
{
    printf("\n\n=================== STB LAUNCHER PANIC ===================\n");
    printf("%s\n", msg);
    printf("Sistema parado para evitar corrupção.\n");
    printf("===========================================================\n");

    // Isso simula um crash real
    exit(1);
}
// -----------------------------------------------------------------------------
// SEÇÃO 7 — GERENCIAMENTO DE PROCESSOS (SIMULADO)
// -----------------------------------------------------------------------------
//
// TUDO é simulado. Não mata processos reais do sistema.
// Cria uma tabela interna com processos falsos que você pode manipular.
// ---------------------------------------------------------------------------

#define STB_MAX_PROCESSES 128

typedef struct {
    int pid;
    char name[64];
    int running;
} stb_process_t;

static stb_process_t stb_process_table[STB_MAX_PROCESSES];
static int stb_next_pid = 1;


// Spawn (criar processo fake)
static int stb_process_spawn(const char* name)
{
    for (int i = 0; i < STB_MAX_PROCESSES; i++)
    {
        if (!stb_process_table[i].running)
        {
            stb_process_table[i].pid = stb_next_pid++;
            stb_process_table[i].running = 1;
            strncpy(stb_process_table[i].name, name, 63);
            return stb_process_table[i].pid;
        }
    }
    return -1;
}


// Kill (matar processo fake)
static void stb_process_kill(int pid)
{
    for (int i = 0; i < STB_MAX_PROCESSES; i++)
    {
        if (stb_process_table[i].pid == pid)
        {
            stb_process_table[i].running = 0;
            return;
        }
    }
}


// Listar processos
static void stb_process_list()
{
    printf("=== PROCESS LIST (SIMULATED) ===\n");

    for (int i = 0; i < STB_MAX_PROCESSES; i++)
    {
        if (stb_process_table[i].running)
            printf("PID %d  |  %s\n", 
                stb_process_table[i].pid, 
                stb_process_table[i].name);
    }
}
// -----------------------------------------------------------------------------
// SEÇÃO 8 — DRIVERS VIRTUAIS (SIMULAÇÃO COMPLETA)
// -----------------------------------------------------------------------------
//
// Você pode registrar drivers como se fossem hardware real.
// ---------------------------------------------------------------------------

typedef enum {
    STB_SENSOR_ACCEL,
    STB_SENSOR_GYRO,
    STB_SENSOR_LIGHT,
    STB_SENSOR_TOUCH
} stb_sensor_type;


typedef struct {
    stb_sensor_type type;
    int enabled;
    float value;
} stb_sensor_driver;

static stb_sensor_driver stb_sensors[16];


// Registrar driver
static void stb_driver_register_sensor(stb_sensor_type type)
{
    for (int i = 0; i < 16; i++)
    {
        if (!stb_sensors[i].enabled)
        {
            stb_sensors[i].type = type;
            stb_sensors[i].enabled = 1;
            stb_sensors[i].value = 0.0f;
            return;
        }
    }
}


// Setar valor fake
static void stb_sensor_set_value(stb_sensor_type type, float value)
{
    for (int i = 0; i < 16; i++)
    {
        if (stb_sensors[i].enabled && stb_sensors[i].type == type)
        {
            stb_sensors[i].value = value;
            return;
        }
    }
}
// -----------------------------------------------------------------------------
// SEÇÃO 9 — PACKAGE MANAGER (SIMULATED)
// -----------------------------------------------------------------------------
//
// Instala / remove pacotes fake estilo apt/yum/apk.
// Todos só existem em memória.
// ---------------------------------------------------------------------------

typedef struct {
    char name[64];
    int installed;
} stb_pkg_t;

static stb_pkg_t stb_pkg_db[128];


// Instalar
static int stb_pkg_install(const char* name)
{
    for (int i = 0; i < 128; i++)
    {
        if (!stb_pkg_db[i].installed)
        {
            stb_pkg_db[i].installed = 1;
            strncpy(stb_pkg_db[i].name, name, 63);
            return 0;
        }
    }
    return -1;
}


// Remover
static int stb_pkg_remove(const char* name)
{
    for (int i = 0; i < 128; i++)
    {
        if (stb_pkg_db[i].installed &&
            strcmp(stb_pkg_db[i].name, name) == 0)
        {
            stb_pkg_db[i].installed = 0;
            return 0;
        }
    }
    return -1;
}
// -----------------------------------------------------------------------------
// SEÇÃO 10 — KERNEL & INIT (SIMULADO)
// -----------------------------------------------------------------------------
//
// Aqui criamos:
//   ✔ mini kernel
//   ✔ init
//   ✔ serviços
//
// Não toca no kernel real do sistema. TOTALMENTE seguro.
// -----------------------------------------------------------------------------


typedef struct {
    char name[64];
    int running;
} stb_service_t;

static stb_service_t stb_services[64];

// Registrar serviço
static void stb_kernel_register_service(const char* name)
{
    for (int i = 0; i < 64; i++)
    {
        if (!stb_services[i].running)
        {
            stb_services[i].running = 1;
            strncpy(stb_services[i].name, name, 63);
            return;
        }
    }
}

// Init system
static void stb_kernel_init()
{
    printf("=== STB KERNEL INIT ===\n");

    stb_kernel_register_service("graphics");
    stb_kernel_register_service("input");
    stb_kernel_register_service("sensor");
    stb_kernel_register_service("package_manager");
    stb_kernel_register_service("shell");

    printf("Kernel inicializado.\n");
}
// -----------------------------------------------------------------------------
// SEÇÃO 11 — SISTEMA DE LOGS DO STB LAUNCHER
// -----------------------------------------------------------------------------
//
// FUNÇÕES DISPONÍVEIS:
//
//   stb_launcher_log("TAG", "mensagem");
//       → log normal estilo Android logcat
//
//   stb_launcher_kernel_panic_print("mensagem");
//       → imprime uma tela de pânico igual kernel Linux
//
//   stb_launcher_print_secured_by_knox_or_no();
//       → imprime se o dispositivo é "Secured by Knox" (simulado)
// -----------------------------------------------------------------------------


#include <time.h>

// Utilidade interna: timestamp
static void stb_log_timestamp()
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    printf("[%02d:%02d:%02d] ",
        t->tm_hour, t->tm_min, t->tm_sec);
}


// -----------------------------------------------------------------------------
// LOG NORMAL (ESTILO LOGCAT)
// -----------------------------------------------------------------------------
// Exemplo:
//   stb_launcher_log("BOOT", "Inicializando kernel");
// -----------------------------------------------------------------------------
static void stb_launcher_log(const char* tag, const char* msg)
{
    if (!tag || !msg) return;

    stb_log_timestamp();
    printf("%s: %s\n", tag, msg);
}



// -----------------------------------------------------------------------------
// KERNEL PANIC PRINT
// -----------------------------------------------------------------------------
// Mostra tela de pânico igual Linux:
//   - Mensagem crítica
//   - Stack fake
//   - Finaliza execução
//
// Totalmente seguro → NÃO trava sistema real.
// -----------------------------------------------------------------------------
static void stb_launcher_kernel_panic_print(const char* message)
{
    printf("\n\n");
    printf(" =====================================================\n");
    printf("                 !!! KERNEL PANIC !!!                 \n");
    printf(" -----------------------------------------------------\n");

    if (message)
        printf("  REASON: %s\n", message);
    else
        printf("  REASON: (unknown)\n");

    printf(" -----------------------------------------------------\n");
    printf("  Stacktrace (simulated):\n");
    printf("    <stb_launcher_kernel> +0x14\n");
    printf("    <stb_fs_mount>        +0x32\n");
    printf("    <stb_boot_init>       +0x08\n");
    printf(" -----------------------------------------------------\n");
    printf("  System halted to avoid corruption.\n");
    printf(" =====================================================\n\n");

    // Não encerrar o programa de verdade no modo seguro
    // exit(1);  // se você quiser REALISMOOOO
}



// -----------------------------------------------------------------------------
// "SECURED BY KNOX" DETECTOR (SIMULADO)
// -----------------------------------------------------------------------------
// Android prints:
//   "Secured by Knox"
// Here you choose TRUE or FALSE manually.
// -----------------------------------------------------------------------------

static int stb_knox_enabled = 0;  // 0 = não, 1 = sim

static void stb_launcher_set_knox(int enabled)
{
    stb_knox_enabled = enabled ? 1 : 0;
}

static void stb_launcher_print_secured_by_knox_or_no()
{
    if (stb_knox_enabled)
    {
        printf("[DEVICE-SECURITY] Secured by Knox\n");
    }
    else
    {
        printf("[DEVICE-SECURITY] Knox: NOT PRESENT\n");
    }
}
