#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>

using namespace std;

SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

bool conectar();
void desconectar();
void mostrarMenuPrincipal();

void menuAlumnos();
void listarAlumnos();
void grabarAlumno();
void modificarAlumno();
void eliminarAlumno();

void menuCursos();
void listarCursos();
void grabarCurso();
void modificarCurso();
void eliminarCurso();

void menuSecciones();
void listarSecciones();
void grabarSeccion();
void modificarSeccion();
void eliminarSeccion();

void menuAsignarCursos();
void asignarCurso();
void listarAsignacionesCursos();
void eliminarAsignacionCurso();

void menuAsignarSeccion();
void asignarSeccion();
void listarAsignacionesSeccion();
void eliminarAsignacionSeccion();

bool conectar() {
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    SQLRETURN ret = SQLConnectA(hDbc, (SQLCHAR*)"UniversidadDSN", SQL_NTS, 
                               (SQLCHAR*)"root", SQL_NTS, 
                               (SQLCHAR*)"admin", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conexion exitosa." << endl;
        return true;
    }
    cout << "Error de conexion." << endl;
    return false;
}

void desconectar() {
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

void mostrarMenuPrincipal() {
    int op;
    do {
        cout << "\n--- SISTEMA UNIVERSIDAD ---" << endl;
        cout << "1. Alumnos\n2. Cursos\n3. Secciones\n4. Alumnos - Cursos\n5. Alumnos - Seccion\n6. Salir" << endl;
        cout << "Seleccione: "; cin >> op;
        switch(op) {
            case 1: menuAlumnos(); break;
            case 2: menuCursos(); break;
            case 3: menuSecciones(); break;
            case 4: menuAsignarCursos(); break;
            case 5: menuAsignarSeccion(); break;
        }
    } while (op != 6);
}

void menuAlumnos() {
    int op;
    do {
        cout << "\nMODULO ALUMNOS\n1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar" << endl;
        cin >> op;
        if (op == 1) listarAlumnos();
        else if (op == 2) grabarAlumno();
        else if (op == 3) modificarAlumno();
        else if (op == 4) eliminarAlumno();
    } while (op != 5);
}

void listarAlumnos() {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    string q = "SELECT a.id, a.nombres, a.apellidos, a.carnet, IFNULL(c.nombre, 'Sin cursos'), IFNULL(s.nombre, 'Sin seccion') "
               "FROM alumnos a LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id LEFT JOIN cursos c ON ac.curso_id = c.id "
               "LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id LEFT JOIN secciones s ON ase.seccion_id = s.id;";
    if (SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS) == SQL_SUCCESS) {
        int id; char n[100], ap[100], car[20], cur[100], sec[100];
        cout << "\nID | Carnet | Nombre | Cursos | Seccion" << endl;
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, n, 100, NULL); SQLGetData(hStmt, 3, SQL_C_CHAR, ap, 100, NULL);
            SQLGetData(hStmt, 4, SQL_C_CHAR, car, 20, NULL); SQLGetData(hStmt, 5, SQL_C_CHAR, cur, 100, NULL);
            SQLGetData(hStmt, 6, SQL_C_CHAR, sec, 100, NULL);
            cout << id << " | " << car << " | " << n << " " << ap << " | " << cur << " | " << sec << endl;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void grabarAlumno() {
    string n, ap, car; cout << "Nombres: "; cin.ignore(); getline(cin, n);
    cout << "Apellidos: "; getline(cin, ap); cout << "Carnet: "; cin >> car;
    string q = "INSERT INTO alumnos (nombres, apellidos, carnet) VALUES ('"+n+"','"+ap+"','"+car+"')";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void modificarAlumno() {
    int id; string n, ap; cout << "ID a modificar: "; cin >> id;
    cout << "Nuevo Nombre: "; cin.ignore(); getline(cin, n); cout << "Nuevo Apellido: "; getline(cin, ap);
    string q = "UPDATE alumnos SET nombres='"+n+"', apellidos='"+ap+"' WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAlumno() {
    int id; cout << "ID a eliminar: "; cin >> id;
    string q = "DELETE FROM alumnos WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuCursos() {
    int op;
    do {
        cout << "\nMODULO CURSOS\n1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar" << endl;
        cin >> op;
        if (op == 1) listarCursos();
        else if (op == 2) grabarCurso();
        else if (op == 3) modificarCurso();
        else if (op == 4) eliminarCurso();
    } while (op != 5);
}

void listarCursos() {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)"SELECT id, nombre, codigo FROM cursos", SQL_NTS);
    int id; char n[100], c[20];
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, n, 100, NULL); SQLGetData(hStmt, 3, SQL_C_CHAR, c, 20, NULL);
        cout << id << " | " << c << " | " << n << endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void grabarCurso() {
    string n, c; cout << "Nombre: "; cin.ignore(); getline(cin, n); cout << "Codigo: "; cin >> c;
    string q = "INSERT INTO cursos (nombre, codigo) VALUES ('"+n+"','"+c+"')";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void modificarCurso() {
    int id; string n; cout << "ID Curso: "; cin >> id;
    cout << "Nuevo Nombre: "; cin.ignore(); getline(cin, n);
    string q = "UPDATE cursos SET nombre='"+n+"' WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarCurso() {
    int id; cout << "ID a eliminar: "; cin >> id;
    string q = "DELETE FROM cursos WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS) != SQL_SUCCESS) 
        cout << "Error: Verifique si hay alumnos asignados." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuSecciones() {
    int op;
    do {
        cout << "\nMODULO SECCIONES\n1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar" << endl;
        cin >> op;
        if (op == 1) listarSecciones();
        else if (op == 2) grabarSeccion();
        else if (op == 3) modificarSeccion();
        else if (op == 4) eliminarSeccion();
    } while (op != 5);
}

void listarSecciones() {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)"SELECT id, nombre, jornada FROM secciones", SQL_NTS);
    int id; char n[50], j[50];
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, n, 50, NULL); SQLGetData(hStmt, 3, SQL_C_CHAR, j, 50, NULL);
        cout << id << " | " << n << " | " << j << endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void grabarSeccion() {
    string n, j; cout << "Nombre: "; cin.ignore(); getline(cin, n); cout << "Jornada: "; getline(cin, j);
    string q = "INSERT INTO secciones (nombre, jornada) VALUES ('"+n+"','"+j+"')";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void modificarSeccion() {
    int id; string j; cout << "ID Seccion: "; cin >> id;
    cout << "Nueva Jornada: "; cin.ignore(); getline(cin, j);
    string q = "UPDATE secciones SET jornada='"+j+"' WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarSeccion() {
    int id; cout << "ID a eliminar: "; cin >> id;
    string q = "DELETE FROM secciones WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuAsignarCursos() {
    int op;
    do {
        cout << "\nALUMNOS - CURSOS\n1. Asignar\n2. Listar\n3. Eliminar\n4. Regresar" << endl;
        cin >> op;
        if (op == 1) asignarCurso();
        else if (op == 2) listarAsignacionesCursos();
        else if (op == 3) eliminarAsignacionCurso();
    } while (op != 4);
}

void asignarCurso() {
    int aid, cid; cout << "ID Alumno: "; cin >> aid; cout << "ID Curso: "; cin >> cid;
    string q = "INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES ("+to_string(aid)+","+to_string(cid)+")";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS) != SQL_SUCCESS)
        cout << "Error: El alumno ya tiene este curso o IDs invalidos." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void listarAsignacionesCursos() {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    string q = "SELECT ac.id, a.nombres, c.nombre FROM alumnos_cursos ac JOIN alumnos a ON ac.alumno_id = a.id JOIN cursos c ON ac.curso_id = c.id";
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    int id; char al[100], cu[100];
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, al, 100, NULL); SQLGetData(hStmt, 3, SQL_C_CHAR, cu, 100, NULL);
        cout << id << " | " << al << " -> " << cu << endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAsignacionCurso() {
    int id; cout << "ID Asignacion a eliminar: "; cin >> id;
    string q = "DELETE FROM alumnos_cursos WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void menuAsignarSeccion() {
    int op;
    do {
        cout << "\nALUMNOS - SECCION\n1. Asignar/Modificar\n2. Listar\n3. Eliminar\n4. Regresar" << endl;
        cin >> op;
        if (op == 1) asignarSeccion();
        else if (op == 2) listarAsignacionesSeccion();
        else if (op == 3) eliminarAsignacionSeccion();
    } while (op != 4);
}

void asignarSeccion() {
    int aid, sid; cout << "ID Alumno: "; cin >> aid; cout << "ID Seccion: "; cin >> sid;
    string q = "REPLACE INTO alumnos_seccion (alumno_id, seccion_id) VALUES ("+to_string(aid)+","+to_string(sid)+")";
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void listarAsignacionesSeccion() {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    string q = "SELECT ase.id, a.nombres, s.nombre FROM alumnos_seccion ase JOIN alumnos a ON ase.alumno_id = a.id JOIN secciones s ON ase.seccion_id = s.id";
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    int id; char al[100], se[100];
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, al, 100, NULL); SQLGetData(hStmt, 3, SQL_C_CHAR, se, 100, NULL);
        cout << id << " | " << al << " -> " << se << endl;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void eliminarAsignacionSeccion() {
    int id; cout << "ID Asignacion a eliminar: "; cin >> id;
    string q = "DELETE FROM alumnos_seccion WHERE id="+to_string(id);
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLExecDirectA(hStmt, (SQLCHAR*)q.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

int main() {
    if (conectar()) {
        mostrarMenuPrincipal();
        desconectar();
    }
    return 0;
}