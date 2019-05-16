## lab2 利用PowerDesigner设计银行业务管理系统的数据库概念模型CDM



### 2.1 实验要求

****

根据下面的需求描述，使用 **Sybase Power Designer** 设计相应的**数据库概念模型**，并转换成**Oracle** 上的**物理数据库结构**。要求提交一个 Word 设计文档，陈述你的设计过程和结果。 

某银行准备开发一个银行业务管理系统，通过调查，得到以下的**主要需求**：

- 银行有多个支行。各个支行位于某个城市，每个支行有唯一的名字。
- 银行要监控每个支行的资产。 
- 银行的客户通过其身份证号来标识。银行存储每个客户的姓名、联系电话以及家庭住址。
- 为了安全起见，银行还要求客户提供一位联系人的信息，包括联系人姓名、手机号、Email 以及与客户的关系。
- 客户可以有帐户，并且可以贷款。
- 客户可能和某个银行员工发生联系，该员工是此客户的贷款负责人或银行帐户负责人。 
- 银行员工也通过身份证号来标识。员工分为部门经理和普通员工，每个部门经理都负责领导其所在部门的员工，并且每个员工只允许在一个部门内工作。
- 每个支行的管理机构存储每个员工的姓名、电话号码、家庭地址、所在的部门号、部门名称、部门类型及部门经理的身份证号。
- 银行还需知道每个员工开始工作的日期，由此日期可以推知员工的雇佣期。 
- 银行提供两类帐户——储蓄帐户和支票帐户。
- 帐户可以由多个客户所共有，一个客户也可开设多个账户，但在一个支行内最多只能开设一
  个储蓄账户和一个支票账户。
- 每个帐户被赋以唯一的帐户号。
- 银行记录每个帐户的余额、开户日期、开户的支行名以及每个帐户所有者访问该帐户的最近日期。
- 另外，每个储蓄帐户有利率和货币类型，且每个支票帐户有透支额。
- 每笔贷款由某个分支机构发放，能被一个或多个客户所共有。
- 每笔贷款用唯一的贷款号标识。
- 银行需要知道每笔贷款所贷金额以及逐次支付的情况（银行将贷款分几次付给客户)。
- 虽然贷款号不能唯一标识银行所有为贷款所付的款项，但可以唯一标识为某贷款所付的款项。
- 对每次的付款需要记录日期和金额。 



### 2.2 设计

****

**1.确定实体**

- **基本实体**

  -支行
  -客户
  -员工
  -账户
  -贷款

- **其他特定对象**

  -领导：属于特殊的员工，但因为需求描述中没有给出标识零的特殊的属性，故不必将其设置为实体，只需要给员工实体一个一对多、指向自己的经理联系即可。 
  -储蓄账户和支票账户：有各自的属性，分别为利率和透支率，故应将这两个对象设置为主体，并继承账户实体。 
  -支付：由于贷款的支付不是一次性的，最好应将支付情况设计为一个单独实体。

**2.确定联系**

- **无属性的联系**

  支行：账户——(1 : N) ——开通

  支行：贷款——(1 : N)——发放

  客户：贷款——(1 : N)——申请

  员工：员工——(1 : N)——领导

  储蓄/支票账户：账户 ——储蓄账户继承/支票账户继承

  贷款：支付——(1 : N)——贷款-支付

- **有属性的联系**

  支行：员工——(1 : N ，开始工作时间)——工作

  员工：客户——(M : N，身份)——负责

  客户：账户——(M : N，最近访问时间)——拥有

**3.设计概念模型CDM**

![概念模型](https://github.com/jocelynzym/Junior/blob/master/Database/lab2/pic/1.png)

**4.概念模型转换为物理模型**

​	![物理模型](https://github.com/jocelynzym/Junior/blob/master/Database/lab2/pic/2.png)

**5.物理模型导出sql脚本**

```sql
/*==============================================================*/
/* DBMS name:      ORACLE Version 11g                           */
/* Created on:     2019/5/16 10:13:03                           */
/*==============================================================*/


alter table "account"
   drop constraint FK_ACCOUNT_开通_BRANCH_B;

alter table "bill_account"
   drop constraint FK_BILL_ACC_BILL_ACC__ACCOUNT;

alter table "charge"
   drop constraint FK_CHARGE_CHARGE_STAFF;

alter table "charge"
   drop constraint FK_CHARGE_CHARGE2_CUSTOM;

alter table "custom"
   drop constraint FK_CUSTOM_APPLY_LOAN;

alter table "custom_account"
   drop constraint FK_CUSTOM_A_CUSTOM_AC_ACCOUNT;

alter table "custom_account"
   drop constraint FK_CUSTOM_A_CUSTOM_AC_CUSTOM;

alter table "loan"
   drop constraint FK_LOAN_LEND_BRANCH_B;

alter table "pay"
   drop constraint FK_PAY_LOAN_PAY_LOAN;

alter table "saving_account"
   drop constraint FK_SAVING_A_SAVING_AC_ACCOUNT;

alter table "staff"
   drop constraint FK_STAFF_LEADER_STAFF;

alter table "staff"
   drop constraint FK_STAFF_WORK_BRANCH_B;

drop index 开通_FK;

drop table "account" cascade constraints;

drop table "bill_account" cascade constraints;

drop table "branch_bank" cascade constraints;

drop index "charge2_FK";

drop index "charge_FK";

drop table "charge" cascade constraints;

drop index "apply_FK";

drop table "custom" cascade constraints;

drop index "custom_account2_FK";

drop index "custom_account_FK";

drop table "custom_account" cascade constraints;

drop index "lend_FK";

drop table "loan" cascade constraints;

drop index "loan_pay_FK";

drop table "pay" cascade constraints;

drop table "saving_account" cascade constraints;

drop index "leader_FK";

drop index "work_FK";

drop table "staff" cascade constraints;



/*==============================================================*/
/* Table: "account"                                             */
/*==============================================================*/
create table "account" 
(
   "account_id"         VARCHAR2(20)         not null,
   "branch_name"        VARCHAR2(50)         not null,
   "balance"            NUMBER(16,1),
   constraint PK_ACCOUNT primary key ("account_id")
);

/*==============================================================*/
/* Index: 开通_FK                                                 */
/*==============================================================*/
create index 开通_FK on "account" (
   "branch_name" ASC
);

/*==============================================================*/
/* Table: "bill_account"                                        */
/*==============================================================*/
create table "bill_account" 
(
   "account_id"         VARCHAR2(20)         not null,
   "branch_name"        VARCHAR2(50),
   "balance"            NUMBER(16,1),
   "credit"             NUMBER(8,3),
   constraint PK_BILL_ACCOUNT primary key ("account_id")
);

/*==============================================================*/
/* Table: "branch_bank"                                         */
/*==============================================================*/
create table "branch_bank" 
(
   "branch_name"        VARCHAR2(50)         not null,
   "city"               VARCHAR2(50),
   "property"           NUMBER(16,1),
   constraint PK_BRANCH_BANK primary key ("branch_name")
);

/*==============================================================*/
/* Table: "charge"                                              */
/*==============================================================*/
create table "charge" 
(
   "staff_id"           CHAR(18)             not null,
   "custom_id"          CHAR(18)             not null,
   "identifier"         VARCHAR2(20),
   constraint PK_CHARGE primary key ("staff_id", "custom_id")
);

/*==============================================================*/
/* Index: "charge_FK"                                           */
/*==============================================================*/
create index "charge_FK" on "charge" (
   "staff_id" ASC
);

/*==============================================================*/
/* Index: "charge2_FK"                                          */
/*==============================================================*/
create index "charge2_FK" on "charge" (
   "custom_id" ASC
);

/*==============================================================*/
/* Table: "custom"                                              */
/*==============================================================*/
create table "custom" 
(
   "custom_id"          CHAR(18)             not null,
   "loan_id"            VARCHAR2(20),
   "name"               VARCHAR2(20),
   "road"               VARCHAR2(50),
   "city"               VARCHAR2(50),
   constraint PK_CUSTOM primary key ("custom_id")
);

/*==============================================================*/
/* Index: "apply_FK"                                            */
/*==============================================================*/
create index "apply_FK" on "custom" (
   "loan_id" ASC
);

/*==============================================================*/
/* Table: "custom_account"                                      */
/*==============================================================*/
create table "custom_account" 
(
   "account_id"         VARCHAR2(20)         not null,
   "custom_id"          CHAR(18)             not null,
   "latest_time"        DATE,
   constraint PK_CUSTOM_ACCOUNT primary key ("account_id", "custom_id")
);

/*==============================================================*/
/* Index: "custom_account_FK"                                   */
/*==============================================================*/
create index "custom_account_FK" on "custom_account" (
   "account_id" ASC
);

/*==============================================================*/
/* Index: "custom_account2_FK"                                  */
/*==============================================================*/
create index "custom_account2_FK" on "custom_account" (
   "custom_id" ASC
);

/*==============================================================*/
/* Table: "loan"                                                */
/*==============================================================*/
create table "loan" 
(
   "loan_id"            VARCHAR2(20)         not null,
   "branch_name"        VARCHAR2(50)         not null,
   "pay_amount"         NUMBER(16,1),
   constraint PK_LOAN primary key ("loan_id")
);

/*==============================================================*/
/* Index: "lend_FK"                                             */
/*==============================================================*/
create index "lend_FK" on "loan" (
   "branch_name" ASC
);

/*==============================================================*/
/* Table: "pay"                                                 */
/*==============================================================*/
create table "pay" 
(
   "loan_id"            VARCHAR2(20)         not null,
   "pay_time"           DATE                 not null,
   "pay_amount"         NUMBER(16,1),
   constraint PK_PAY primary key ("loan_id", "pay_time")
);

/*==============================================================*/
/* Index: "loan_pay_FK"                                         */
/*==============================================================*/
create index "loan_pay_FK" on "pay" (
   "loan_id" ASC
);

/*==============================================================*/
/* Table: "saving_account"                                      */
/*==============================================================*/
create table "saving_account" 
(
   "account_id"         VARCHAR2(20)         not null,
   "branch_name"        VARCHAR2(50),
   "balance"            NUMBER(16,1),
   "rate"               NUMBER(8,3),
   constraint PK_SAVING_ACCOUNT primary key ("account_id")
);

/*==============================================================*/
/* Table: "staff"                                               */
/*==============================================================*/
create table "staff" 
(
   "staff_id"           CHAR(18)             not null,
   "sta_staff_id"       CHAR(18),
   "branch_name"        VARCHAR2(50)         not null,
   "name"               VARCHAR2(20),
   "tel"                VARCHAR2(20),
   "addr"               VARCHAR2(50),
   "work_start"         DATE,
   constraint PK_STAFF primary key ("staff_id")
);

/*==============================================================*/
/* Index: "work_FK"                                             */
/*==============================================================*/
create index "work_FK" on "staff" (
   "branch_name" ASC
);

/*==============================================================*/
/* Index: "leader_FK"                                           */
/*==============================================================*/
create index "leader_FK" on "staff" (
   "sta_staff_id" ASC
);

alter table "account"
   add constraint FK_ACCOUNT_开通_BRANCH_B foreign key ("branch_name")
      references "branch_bank" ("branch_name");

alter table "bill_account"
   add constraint FK_BILL_ACC_BILL_ACC__ACCOUNT foreign key ("account_id")
      references "account" ("account_id");

alter table "charge"
   add constraint FK_CHARGE_CHARGE_STAFF foreign key ("staff_id")
      references "staff" ("staff_id");

alter table "charge"
   add constraint FK_CHARGE_CHARGE2_CUSTOM foreign key ("custom_id")
      references "custom" ("custom_id");

alter table "custom"
   add constraint FK_CUSTOM_APPLY_LOAN foreign key ("loan_id")
      references "loan" ("loan_id");

alter table "custom_account"
   add constraint FK_CUSTOM_A_CUSTOM_AC_ACCOUNT foreign key ("account_id")
      references "account" ("account_id");

alter table "custom_account"
   add constraint FK_CUSTOM_A_CUSTOM_AC_CUSTOM foreign key ("custom_id")
      references "custom" ("custom_id");

alter table "loan"
   add constraint FK_LOAN_LEND_BRANCH_B foreign key ("branch_name")
      references "branch_bank" ("branch_name");

alter table "pay"
   add constraint FK_PAY_LOAN_PAY_LOAN foreign key ("loan_id")
      references "loan" ("loan_id");

alter table "saving_account"
   add constraint FK_SAVING_A_SAVING_AC_ACCOUNT foreign key ("account_id")
      references "account" ("account_id");

alter table "staff"
   add constraint FK_STAFF_LEADER_STAFF foreign key ("sta_staff_id")
      references "staff" ("staff_id");

alter table "staff"
   add constraint FK_STAFF_WORK_BRANCH_B foreign key ("branch_name")
      references "branch_bank" ("branch_name");

```

**6.在Oracle中测试脚本运行情况**

在oracle中新建sql脚本窗口, 将以上代码复制进去，点击执行即可。需要注意的是，在第一次执行该脚本时，create table前的drop操作应注释掉。以后执行该脚本时，取消注释。因为在第一次执行脚本时还没有创建表。



### 2.3 设计工具

****

- Sybase PowerDesigner 16.5



### 2.4 实验参考

****

- [数据库设计---PowerDesigner（概念模型和物理模型）](https://blog.csdn.net/business122/article/details/82771871)
- [powerdesign破解](https://www.cnblogs.com/hjwq/p/8405626.html)
- [CDM设计步骤图解](https://jingyan.baidu.com/article/b87fe19e78ecb9521935684c.html)
- [PowerDesigner中添加表间关系](https://wenku.baidu.com/view/e57c230d03020740be1e650e52ea551810a6c93a.html)
- [概念模型->物理模型->sql脚本](https://zhidao.baidu.com/question/2266098784380407828.html)