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

int main() 
{
    struct CMUnitTest tests[] = {
        cmocka_unit_test(TestParseSingleLine),
        cmocka_unit_test(TestParseElementInBody)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
