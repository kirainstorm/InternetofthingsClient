// Write your JavaScript code.

function Fun_loginValidate() {
	$("#form-box").validate({
		debug: true, //debug，只验证不提交表单
		rules: {
			usernametext: {
				required: true
			},
			pwdtext: {
				required: true
			}
		},
		messages: {
			usernametext: {
				required: _msg_error_user_name_1
			},
			pwdtext: {
				required: _msg_error_user_pwd_1
			}
		},
		showErrors: function (errorMap, errorList) {
			$("#loginError span").remove();
			if (0 == errorList.length) {
				$("#loginError").hide();
			} else {
				var msg = "";
				for (j = 0; j < errorList.length; j++) {
					//alert(errorList[j].message);
					msg += errorList[j].message;
					if (j < (errorList.length - 1))
						msg += '<br/>'
				}
				//$("#loginError").append('<span>'+errorList[0].message+'</span>');
				$("#loginError").append('<span>' + msg + '</span>');
				$("#loginError").show();
			}
		}
	});
}
