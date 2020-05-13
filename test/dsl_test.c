#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>

#include <cmocka.h>

#include "dsl.h"

void TestParseSingleLine(void** state)
{
    char testConfig[] = "Name(args){body}";

    int ret;
    DslElement* dsl = NULL;

    ret = DslParse(testConfig, &dsl);
    assert_int_equal(ret, 0);
    assert_non_null(dsl);

    assert_int_equal(dsl->name->type, TYPE_VAR);
    assert_string_equal(dsl->name->var, "Name");

    assert_int_equal(dsl->args->type, TYPE_VAR);
    assert_string_equal(dsl->args->var, "args");

    assert_int_equal(dsl->body->type, TYPE_VAR);
    assert_string_equal(dsl->body->var, "body");

    free(dsl);
}

void TestParseElementInBody(void** state)
{
    char testConfig[] = "Name(){body(args){abcabcabc}}";

    int ret;
    DslElement* dsl = NULL;

    ret = DslParse(testConfig, &dsl);
    assert_int_equal(ret, 0);
    assert_non_null(dsl);

    assert_int_equal(dsl->name->type, TYPE_VAR);
    assert_string_equal(dsl->name->var, "Name");

    assert_int_equal(dsl->body->type, TYPE_ELEMENT);

    DslElement* body = dsl->body->element;
    assert_int_equal(body->name->type, TYPE_VAR);
    assert_string_equal(body->name->var, "body");

    assert_int_equal(body->args->type, TYPE_VAR);
    assert_string_equal(body->args->var, "args");

    assert_int_equal(body->body->type, TYPE_VAR);
    assert_string_equal(body->body->var, "abcabcabc");

    free(dsl);
}


void TestParseMultiValue(void** state)
{
    char testConfig[] = "Name(1, 2, 3){4,5,6}";

    int ret;
    DslElement* dsl;
    ret = DslParse(testConfig, &dsl);
    assert_int_equal(ret, 0);
    assert_non_null(dsl);

    assert_int_equal(dsl->args->number, 1);
    assert_int_equal(dsl->args->next->number, 2);
    assert_int_equal(dsl->args->next->next->number, 3);

    assert_int_equal(dsl->body->number, 4);
    assert_int_equal(dsl->body->next->number, 5);
    assert_int_equal(dsl->body->next->next->number, 6);
}

void TestParseMultiValueInBody(void** state)
{
    char testConfig[] = {
        "Scope(_SB) {"
            "Name(_HID,1) {},"
            "Name(_UID,1) {}"
            "}"
    };

    int ret;
    DslElement* dsl;
    ret = DslParse(testConfig, &dsl);
    assert_int_equal(ret, 0);
    assert_non_null(dsl);

    assert_string_equal(dsl->name->var, "Scope");
    assert_string_equal(dsl->args->var, "_SB");

    assert_non_null(dsl->body);
    assert_non_null(dsl->body->next);

    DslElement* hid, *uid;
    hid = dsl->body->element;
    uid = dsl->body->next->element;

    assert_string_equal(hid->name->var, "Name");
    assert_string_equal(hid->args->var, "_HID");
    assert_int_equal(hid->args->next->number, 1);

    assert_string_equal(uid->name->var, "Name");
    assert_string_equal(uid->args->var, "_UID");
    assert_int_equal(uid->args->next->number, 1);

}

int main() 
{
    struct CMUnitTest tests[] = {
        cmocka_unit_test(TestParseSingleLine),
        cmocka_unit_test(TestParseElementInBody),
        cmocka_unit_test(TestParseMultiValue),
        cmocka_unit_test(TestParseMultiValueInBody),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
